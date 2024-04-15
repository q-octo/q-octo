#include "config.h"
#include <Arduino.h> // Always include this first
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "CRSFforArduino.hpp"
#include "task_rc.h"
#include <Arduino-CRSF.h>

#define FULL_CRSF_FEATURES 1
#define DEBUG_LOG_RC_CHANNELS 1
#define DEBUG_LOG_RC_LINK_STATS 1
#define BROADCAST_FREQUENCY 500 // ms

#define TICKS_TO_US(x) ((x - 992) * 5 / 8 + 1500)
#define mapRange(a1, a2, b1, b2, s) (b1 + (s - a1) * (b2 - b1) / (a2 - a1))

CRSFforArduino *crsf = nullptr;

/* A flag to hold the fail-safe status. */
bool isFailsafeActive = false;

/* RC Channels data. */
int rcChannelCount = 8;
// 16 channels
const char *rcChannelNames[] = {
    "A",
    "E",
    "T",
    "R",
    "Aux1",
    "Aux2",
    "Aux3",
    "Aux4",

    "Aux5", // Failsafe Channel
    "Aux6",
    "Aux7",
    "Aux8",
    "Aux9",
    "Aux10",
    "Aux11",
    "Aux12"};

QueueHandle_t rcSendQueue;
TaskMessage::Message taskMessage;

#define RC_CHANNELS_LOG_FREQUENCY 2000   // ms
#define RC_LINK_STATS_LOG_FREQUENCY 2000 // ms
uint32_t lastRcChannelsLogMs = 0;
uint32_t lastRcLinkStatsLogMs = 0;
uint32_t lastBroadcastMs = 0;
float lastRPM = 0;

void terminateCrsf();
/* RC Channels Event Callback. */
// void onReceiveRcChannels(serialReceiverLayer::rcChannels_t *rcData);
void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t);
void onReceiveChannels(const uint16_t channels[]);
void onFailsafeActivated();
void onFailsafeCleared();

CRSF *crsf2 = nullptr;

void taskReceiveFromRC(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings
    Serial.println("taskReceiveFromRC started");
    // Serial1.setTX(12);
    // Serial1.setRX(13);

    // Serial1 = UART0, Serial2 = UART1.
    Serial2.setTX(CFG_RC_UART_TX);
    Serial2.setRX(CFG_RC_UART_RX);
#if FULL_CRSF_FEATURES
    crsf = new CRSFforArduino(&Serial2);

    /* Initialise CRSF for Arduino, and clean up
    any allocated resources if initialisation fails. */
    if (crsf->begin())
    {
        crsf->setLinkStatisticsCallback(onLinkStatisticsUpdate);
        crsf->setRcChannelsCallback([](serialReceiverLayer::rcChannels_t *rcData)
                                    {
            if (rcData->failsafe) { if (!isFailsafeActive) onFailsafeActivated(); }
            else {
            /* Set the failsafe status to false. */
            if (isFailsafeActive) { onFailsafeCleared(); }
            onReceiveChannels(rcData->value);
        } });
        /* Constrain the RC Channels Count to the maximum number
          of channels that are specified by The Crossfire Protocol.*/
        rcChannelCount = rcChannelCount > crsfProtocol::RC_CHANNEL_COUNT ? crsfProtocol::RC_CHANNEL_COUNT : rcChannelCount;
    }
    else
    {
        /* CRSF for Arduino failed to initialise.
        Clean-up the resources that it previously allocated, and then free up the memory it allocated. */
        Serial.println("CRSF init failed.");
        terminateCrsf();
        return;
    }
#else
    crsf2 = new CRSF();
    crsf2->begin(&Serial2, 420000);
    isFailsafeActive = !crsf2->isConnected();
    crsf2->onDataReceived([](const uint16_t channels[])
                          {
        if (isFailsafeActive)
        {
            onFailsafeCleared();
        } 
        onReceiveChannels(channels); });
    crsf2->onDisconnected(onFailsafeActivated);
#endif
    for (;;)
    {
#if FULL_CRSF_FEATURES
        crsf->update();
#else
        crsf2->readPacket();
#endif
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void taskSendToRC(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings
    Serial.println("taskSendToRC started");
    rcSendQueue = xQueueCreate(10, sizeof(TaskRC::Message));
    if (rcSendQueue == nullptr)
    {
        Serial.println("Failed to create rcSendQueue");
        vTaskDelete(nullptr);
    }

    TaskRC::Message message;
#if !CFG_ENABLE_RC
    Serial.println("RC disabled, blocking indefinitely");
    for (;;)
    {
        xQueueReceive(rcSendQueue, &message, portMAX_DELAY);
    }
#endif

#if !FULL_CRSF_FEATURES
    // Unable to send telemetry data, block indefinitely
    for (;;)
    {
        xQueueReceive(rcSendQueue, &message, portMAX_DELAY);
    }
#else
    for (;;)
    {
        if (xQueueReceive(rcSendQueue, &message, portMAX_DELAY))
        {
            switch (message.type)
            {
            case TaskRC::BATTERY:
                // TODO once pico_crsf PR is merged, update these.
                // * current and voltage by 10.
                crsf->telemetryWriteBattery(
                    message.as.battery.voltage,
                    message.as.battery.current,
                    message.as.battery.fuel,
                    42);
                break;
            default:
                Serial.println("[ERROR] unknown message type");
                break;
            }
        }
    }
#endif
}

void terminateCrsf()
{
#if FULL_CRSF_FEATURES
    crsf->end();
    delete crsf;
    crsf = nullptr;
#else
    delete crsf2;
    crsf2 = nullptr;
#endif
}

void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics)
{

    const uint32_t currentMillis = millis();
#if DEBUG_LOG_RC_LINK_STATS
    if (currentMillis - lastRcLinkStatsLogMs >= RC_LINK_STATS_LOG_FREQUENCY)
    {
        lastRcLinkStatsLogMs = currentMillis;
        Serial.print("Link Statistics: ");
        Serial.print("RSSI: ");
        Serial.print(linkStatistics.rssi);
        Serial.print(", Link Quality: ");
        Serial.print(linkStatistics.lqi);
        Serial.print(", Signal-to-Noise Ratio: ");
        Serial.print(linkStatistics.snr);
        Serial.print(", Transmitter Power: ");
        Serial.println(linkStatistics.tx_power);
    }
#endif

    if (currentMillis - lastBroadcastMs >= BROADCAST_FREQUENCY)
    {
        lastBroadcastMs = currentMillis;
        taskMessage = {
            .type = TaskMessage::Type::STATE_RC,
            .as = {
                .rc = {
                    .rssi = linkStatistics.rssi,
                    .linkQuality = linkStatistics.lqi,
                    .signalNoiseRatio = linkStatistics.snr,
                    .tx_power = linkStatistics.tx_power,
                },
            },
        };
        xQueueSend(dataManagerQueue, &taskMessage, 0);
    }
}

void onReceiveChannels(const uint16_t channels[])
{
#if DEBUG_LOG_RC_CHANNELS
    const uint32_t currentMillis = millis();
    if (currentMillis - lastRcChannelsLogMs >= RC_CHANNELS_LOG_FREQUENCY && !isFailsafeActive)
    {
        lastRcChannelsLogMs = currentMillis;

        Serial.print("[INFO]: RC Channels: <");
        for (int i = 0; i < rcChannelCount; i++)
        {
            Serial.print(rcChannelNames[i]);
            Serial.print(": ");
            Serial.print(TICKS_TO_US(channels[i]));

            if (i < (rcChannelCount - 1))
            {
                Serial.print(", ");
            }
        }
        Serial.println(">");
    }
#endif

    // float direction = mapRange(992, 2008, -1, 1, crsf->rcToUs(rcData->value[1]));
    float rpm = mapRange(992, 2008, -10, 10, TICKS_TO_US(channels[0]));
    if (lastRPM != rpm)
    {
        lastRPM = rpm;
        taskMessage = {
            .type = TaskMessage::Type::SET_MOTOR_SPEED_COMBINED,
            .as = {.motorSpeedCombined = {.rpm = rpm, .direction = 0}},
        };
        xQueueSend(dataManagerQueue, &taskMessage, 0);
    }
}

void onFailsafeActivated()
{
    if (isFailsafeActive)
    {
        return;
    }
    isFailsafeActive = true;
    Serial.println("[WARN]: Failsafe detected.");
    taskMessage = {.type = TaskMessage::Type::TX_LOST};
    xQueueSend(dataManagerQueue, &taskMessage, 0);
}
void onFailsafeCleared()
{
    if (!isFailsafeActive)
    {
        return;
    }
    isFailsafeActive = false;
    Serial.println("[INFO]: Failsafe cleared.");
    taskMessage = {.type = TaskMessage::Type::TX_RESTORED};
    xQueueSend(dataManagerQueue, &taskMessage, 0);
}