#include "config.h"
#include <Arduino.h> // Always include this first
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "task_rc.h"
#include "crsf.h"

#define DEBUG_LOG_RC_CHANNELS 1
#define DEBUG_LOG_RC_LINK_STATS 1
#define BROADCAST_FREQUENCY 500 // ms

#define mapRange(a1, a2, b1, b2, s) (b1 + (s - a1) * (b2 - b1) / (a2 - a1))

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

/* RC Channels Event Callback. */
void onLinkStatisticsUpdate(const link_statistics_t linkStats);
void onReceiveChannels(const uint16_t channels[16]);
void onFailsafe(const bool failsafe);

void taskReceiveFromRC(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings
    Serial.println("taskReceiveFromRC started");

    crsf_set_link_quality_threshold(70);
    crsf_set_rssi_threshold(105);
    crsf_set_on_link_statistics(onLinkStatisticsUpdate);
    crsf_set_on_rc_channels(onReceiveChannels);
    crsf_set_on_failsafe(onFailsafe);
    crsf_begin(uart1, CFG_RC_UART_TX, CFG_RC_UART_RX);

    for (;;)
    {
        crsf_process_frames();
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

    static TaskRC::Message message;
#if !CFG_ENABLE_RC
    Serial.println("RC disabled, blocking indefinitely");
    for (;;)
    {
        xQueueReceive(rcSendQueue, &message, portMAX_DELAY);
    }
#endif

    for (;;)
    {
        if (xQueueReceive(rcSendQueue, &message, portMAX_DELAY))
        {
            switch (message.type)
            {
            case TaskRC::BATTERY:
                crsf_telem_set_battery_data(
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
}

void onLinkStatisticsUpdate(const link_statistics_t linkStatistics)
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
        Serial.print(linkStatistics.link_quality);
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
                    .linkQuality = linkStatistics.link_quality,
                    .signalNoiseRatio = linkStatistics.snr,
                    .tx_power = linkStatistics.tx_power,
                },
            },
        };
        xQueueSend(dataManagerQueue, &taskMessage, 0);
    }
}

void onReceiveChannels(const uint16_t channels[16])
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

void onFailsafe(const bool failsafe)
{
    if (failsafe)
    {
        onFailsafeActivated();
    }
    else
    {
        onFailsafeCleared();
    }
}