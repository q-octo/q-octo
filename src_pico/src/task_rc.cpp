#include "config.h"
#include <Arduino.h> // Always include this first
#include "task_rc.h"

#define DEBUG_LOG_RC_CHANNELS 1
#define DEBUG_LOG_RC_LINK_STATS 1
#define BROADCAST_FREQUENCY 500 // ms

#define mapRange(a1, a2, b1, b2, s) (b1 + (s - a1) * (b2 - b1) / (a2 - a1))

DataManager::Message taskMessage;
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

uint32_t lastRcChannelsLogMs = 0;
uint32_t lastRcLinkStatsLogMs = 0;
uint32_t lastBroadcastMs = 0;
float lastRPM = 0;

#define RC_CHANNELS_LOG_FREQUENCY 2000   // ms
#define RC_LINK_STATS_LOG_FREQUENCY 2000 // ms

void TaskRC::init() {
  if (!CFG_ENABLE_RC) {
    return;
  }

  crsf_set_link_quality_threshold(70);
  crsf_set_rssi_threshold(105);
  crsf_set_on_link_statistics(onLinkStatisticsUpdate);
  crsf_set_on_rc_channels(onReceiveChannels);
  crsf_set_on_failsafe(onFailsafe);
  crsf_begin(uart0, CFG_RC_UART_TX, CFG_RC_UART_RX);

  Serial.println("RC class init complete");
}

void TaskRC::loop() {
  if (!CFG_ENABLE_RC) {
    return;
  }
  crsf_process_frames();
}

void TaskRC::receiveMessage(const Message &message) {

  switch (message.type) {
    case TaskRC::BATTERY:
      crsf_telem_set_battery_data(
              message.as.battery.voltage * 10,
              message.as.battery.current * 10,
              message.as.battery.fuel,
              42);
      break;
    default:
      Serial.println("[ERROR] unknown message type");
      break;
  }
}

void TaskRC::onLinkStatisticsUpdate(const link_statistics_t linkStatistics) {
  const uint32_t currentMillis = millis();
#if DEBUG_LOG_RC_LINK_STATS
  if (currentMillis - lastRcLinkStatsLogMs >= RC_LINK_STATS_LOG_FREQUENCY) {
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

  if (currentMillis - lastBroadcastMs >= BROADCAST_FREQUENCY) {
    lastBroadcastMs = currentMillis;
    taskMessage = {
            .type = DataManager::Type::STATE_RC,
            .as = {
                    .rc = {
                            .rssi = linkStatistics.rssi,
                            .linkQuality = linkStatistics.link_quality,
                            .signalNoiseRatio = linkStatistics.snr,
                            .tx_power = linkStatistics.tx_power,
                    },
            },
    };
    DataManager::receiveMessage(taskMessage);
  }
}

void TaskRC::onReceiveChannels(const uint16_t channels[16]) {
#if DEBUG_LOG_RC_CHANNELS
  const uint32_t currentMillis = millis();
  if (currentMillis - lastRcChannelsLogMs >= RC_CHANNELS_LOG_FREQUENCY && !isFailsafeActive) {
    lastRcChannelsLogMs = currentMillis;

    Serial.print("[INFO]: RC Channels: <");
    for (int i = 0; i < rcChannelCount; i++) {
      Serial.print(rcChannelNames[i]);
      Serial.print(": ");
      Serial.print(TICKS_TO_US(channels[i]));

      if (i < (rcChannelCount - 1)) {
        Serial.print(", ");
      }
    }
    Serial.println(">");
  }
#endif

  // float direction = mapRange(992, 2008, -1, 1, crsf->rcToUs(rcData->value[1]));
  float rpm = mapRange(992, 2008, -10, 10, TICKS_TO_US(channels[0]));
  if (lastRPM != rpm) {
    lastRPM = rpm;
    taskMessage = {
            .type = DataManager::Type::SET_MOTOR_SPEED_COMBINED,
            .as = {.motorSpeedCombined = {.rpm = rpm, .direction = 0}},
    };
    DataManager::receiveMessage(taskMessage);
  }
}

void TaskRC::onFailsafeActivated() {
  if (isFailsafeActive) {
    return;
  }
  isFailsafeActive = true;
  Serial.println("[WARN]: Failsafe detected.");
  taskMessage = {.type = DataManager::Type::TX_LOST};
  DataManager::receiveMessage(taskMessage);
}

void TaskRC::onFailsafeCleared() {
  if (!isFailsafeActive) {
    return;
  }
  isFailsafeActive = false;
  Serial.println("[INFO]: Failsafe cleared.");
  taskMessage = {.type = DataManager::Type::TX_RESTORED};
  DataManager::receiveMessage(taskMessage);
}

void TaskRC::onFailsafe(const bool failsafe) {
  if (failsafe) {
    onFailsafeActivated();
  } else {
    onFailsafeCleared();
  }
}