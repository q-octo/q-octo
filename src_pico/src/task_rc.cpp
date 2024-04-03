#include <Arduino.h> // Always include this first
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "CRSFforArduino.hpp"
#include "task_rc.h"

#define mapRange(a1, a2, b1, b2, s) (b1 + (s - a1) * (b2 - b1) / (a2 - a1))
#define DEBUG_LOG_RC_CHANNELS 1
#define DEBUG_LOG_RC_LINK_STATS 1
#define BROADCAST_FREQUENCY 500 // ms

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
TaskMessage taskMessage;

#define RC_CHANNELS_LOG_FREQUENCY 500   // ms
#define RC_LINK_STATS_LOG_FREQUENCY 500 // ms
uint32_t lastRcChannelsLogMs = 0;
uint32_t lastRcLinkStatsLogMs = 0;
uint32_t lastBroadcastMs = 0;

void terminateCrsf();
/* RC Channels Event Callback. */
void onReceiveRcChannels(serialReceiverLayer::rcChannels_t *rcData);
void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t);

void taskReceiveFromRC(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings
    Serial.println("taskCRSF started");
    crsf = new CRSFforArduino();

    /* Initialise CRSF for Arduino, and clean up
    any allocated resources if initialisation fails. */
    if (crsf->begin())
    {
        crsf->setLinkStatisticsCallback(onLinkStatisticsUpdate);
        crsf->setRcChannelsCallback(onReceiveRcChannels);
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

    for (;;)
    {
        crsf->update();
        // TODO consider a 1ms delay?
        delay(pdMS_TO_TICKS(10));
    }
}

void taskSendToRC(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings
    Serial.println("taskSendToRC started");
    rcSendQueue = xQueueCreate(10, sizeof(TaskRCMessage));
    if (rcSendQueue == nullptr)
    {
        Serial.println("Failed to create rcSendQueue");
        vTaskDelete(nullptr);
    }

    TaskRCMessage message;
    for (;;)
    {
        if (xQueueReceive(rcSendQueue, &message, portMAX_DELAY))
        {
            switch (message.type)
            {
            case TaskRC::BATTERY:
                crsf->telemetryWriteBattery(
                    message.as.battery.voltage,
                    message.as.battery.current,
                    message.as.battery.fuel,
                    100);
                break;
            default:
                Serial.println("[ERROR] unknown message type");
                break;
            }
        }
    }
}

void terminateCrsf()
{
    crsf->end();
    delete crsf;
    crsf = nullptr;
}

void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics)
{
    // WARNING: Treat as an ISR callback!

    // portYIELD_FROM_ISR requests a context switch to the highest priority task
    // that is ready to run.
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static const uint32_t currentMillis = millis();
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
            .type = TaskMessageType::STATE_RC,
            .rc = {
                .rssi = linkStatistics.rssi,
                .linkQuality = linkStatistics.lqi,
                .signalNoiseRatio = linkStatistics.snr,
                .tx_power = linkStatistics.tx_power,
            }};
        xQueueSendFromISR(dataManagerQueue, &taskMessage, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void onReceiveRcChannels(serialReceiverLayer::rcChannels_t *rcData)
{
    // WARNING: Treat as an ISR callback!

    /* This is your RC Channels Event Callback.
    Here, you have access to all 16 11-bit channels,
    plus an additional "failsafe" flag that tells you whether-or-not
    your receiver is connected to your controller's transmitter module.

    Using the rcData parameter that was passed in,
    you have access to the following:

    - failsafe - A boolean flag indicating the "Fail-safe" status.
    - value[16] - An array consisting of all 16 received RC channels.
      NB: RC Channels are RAW values and are NOT in "microseconds" units.
    */

    // portYIELD_FROM_ISR requests a context switch to the highest priority task
    // that is ready to run.
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (rcData->failsafe)
    {
        if (!isFailsafeActive)
        {
            isFailsafeActive = true;
            Serial.println("[WARN]: Failsafe detected.");
            taskMessage = {.type = TaskMessageType::TX_LOST};
            xQueueSendFromISR(dataManagerQueue, &taskMessage, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else
    {
        /* Set the failsafe status to false. */
        if (isFailsafeActive)
        {
            isFailsafeActive = false;
            Serial.println("[Sketch | INFO]: Failsafe cleared.");
            taskMessage = {.type = TaskMessageType::TX_RESTORED};
            xQueueSendFromISR(dataManagerQueue, &taskMessage, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        else
        {
            float motorRPM = mapRange(992, 2008, -30, 30, crsf->rcToUs(rcData->value[0]));
            float direction = mapRange(992, 2008, -1, 1, crsf->rcToUs(rcData->value[1]));
            taskMessage = {
                .type = TaskMessageType::SET_MOTOR_SPEED_INDIVIDUAL,
                .motorSpeedCombined = {
                    .rpm = motorRPM,
                    .direction = direction,
                },
            };
            xQueueSendFromISR(dataManagerQueue, &taskMessage, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }

#if DEBUG_LOG_RC_CHANNELS
    static const uint32_t currentMillis = millis();
    if (currentMillis - lastRcChannelsLogMs >= RC_CHANNELS_LOG_FREQUENCY)
    {
        lastRcChannelsLogMs = currentMillis;

        Serial.print("[INFO]: RC Channels: <");
        for (int i = 0; i < rcChannelCount; i++)
        {
            Serial.print(rcChannelNames[i]);
            Serial.print(": ");
            Serial.print(crsf->rcToUs(rcData->value[i]));

            if (i < (rcChannelCount - 1))
            {
                Serial.print(", ");
            }
        }
        Serial.println(">");
    }
#endif
}
