#include "config.h"
#include <Arduino.h> // Always include this first
#include "task_rc.h"
#include "system_status.h"

#define DEBUG_LOG_RC_CHANNELS 1
#define DEBUG_LOG_RC_LINK_STATS 1
#define BROADCAST_FREQUENCY 500 // ms

#define mapRange(a1, a2, b1, b2, s) (b1 + (s - a1) * (b2 - b1) / (a2 - a1))
#define RC_CHANNELS_LOG_FREQUENCY 2000   // ms
#define RC_LINK_STATS_LOG_FREQUENCY 2000 // ms

void TaskRC::setThresholds() {
  crsf_set_link_quality_threshold(state.linkQualityThreshold);
  crsf_set_rssi_threshold(state.rssiThreshold);
}

void TaskRC::init() {
  if (!CFG_ENABLE_RC) {
    return;
  }

  setThresholds();
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
  const uint32_t currentMillis = millis();
  if (currentMillis - lastBroadcastMs >= BROADCAST_FREQUENCY) {
    lastBroadcastMs = currentMillis;
    DataManager::RC rc = {
            .rssi = lastLinkStats.rssi,
            .linkQuality = lastLinkStats.link_quality,
            .signalNoiseRatio = lastLinkStats.snr,
            .tx_power = lastLinkStats.tx_power,
            .channels = &lastChannelsUs[0],
            .failsafe = isFailsafeActive,
    };
    taskMessage = {.type = DataManager::Type::STATE_RC, .as = {.rc = rc}};
    DataManager::receiveMessage(taskMessage);
  }

  const StorageState& state = Storage::getState();

  // Seems like we receive updates more frequently when the receiver is connected
  const bool hasReceiverTimedOut = currentMillis - lastReceiverUpdateMs >= state.radioReceiverTimeoutMillis;  
  if (receiverTimedOut != hasReceiverTimedOut) {
    receiverTimedOut = hasReceiverTimedOut;
    if (receiverTimedOut) {
      onFailsafeActivated();
    } else {
      onFailsafeCleared();
    }
  }
}

void TaskRC::receiveMessage(const Message &message) {

  switch (message.type) {
    case BATTERY:
      crsf_telem_set_battery_data(
              message.as.battery.voltage * 10,
              message.as.battery.current * 10,
              message.as.battery.fuel,
              message.as.battery.percent);
      break;
    case STATE_UPDATE:
      setThresholds();
      sendStateAsTelem(message.as.state);
      break;
    default:
      Serial.println("[ERROR] unknown message type");
      break;
  }
}

void TaskRC::sendStateAsTelem(const DataManager::State &state) {
  const size_t payloadSize = 14;
  uint8_t buffer[payloadSize] = {0};
  buffer[0] = 1; // Our internal message type.
  buffer[1] = state.webServerEnabled;
  buffer[2] = state.armed;
  // Manual, obc, flight con
  buffer[3] = state.controlSource;
  buffer[4] = SystemStatus::getStatus();
  auto speedLimit = static_cast<uint16_t>(max(state.leftMotorLimits.max_speed, state.rightMotorLimits.max_speed) * 10);
  // little endian
  uint16_t* speedLimitPtr = reinterpret_cast<uint16_t*>(&buffer[5]);
  *speedLimitPtr = speedLimit; 
  uint16_t currentLimit = static_cast<uint16_t>(max(state.leftMotorLimits.max_current, state.rightMotorLimits.max_current) * 10);
  uint16_t* currentLimitPtr = reinterpret_cast<uint16_t*>(&buffer[7]);
  *currentLimitPtr = currentLimit; 
  buffer[9] = static_cast<uint8_t>(max(state.leftMotorLimits.max_torque, state.rightMotorLimits.max_torque) * 10);
  uint16_t speedKp = static_cast<uint16_t>(state.leftMotorLimits.speed_kp * 10);
  uint16_t* speedKpPtr = reinterpret_cast<uint16_t*>(&buffer[10]);
  *speedKpPtr = speedKp;
  uint16_t speedKi = static_cast<uint16_t>(state.leftMotorLimits.speed_ki * 10);
  uint16_t* speedKiPtr = reinterpret_cast<uint16_t*>(&buffer[12]);
  *speedKiPtr = speedKi;

  /*
  crsfPayload = {
  1,    -- Message Type
  0,    -- Wifi
  0,    -- Armed
  0,    -- Control Source
  -1,    -- Status
  0x00, 0x00, -- Speed Limit
  0x00, 0x00, -- Current Limit
  0,    -- Torque Limit
  0x00, 0x00, -- Speed Kp
  0x00, 0x00, -- Speed Ki
}
  */
  crsf_telem_set_custom_payload(buffer, payloadSize);
}

void TaskRC::onLinkStatisticsUpdate(const link_statistics_t linkStatistics) {
  const uint32_t currentMillis = millis();
  lastReceiverUpdateMs = currentMillis;
  lastLinkStats = linkStatistics;
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
}

void TaskRC::onReceiveChannels(const uint16_t channels[16]) {
  memcpy(lastChannels, channels, sizeof(lastChannels));
  for (int i = 0; i < 16; i++) {
    lastChannelsUs[i] = TICKS_TO_US(channels[i]);
  }
#if DEBUG_LOG_RC_CHANNELS
  const uint32_t currentMillis = millis();
  if (currentMillis - lastRcChannelsLogMs >= RC_CHANNELS_LOG_FREQUENCY) {
    lastRcChannelsLogMs = currentMillis;

    if (isFailsafeActive) {
      Serial.print("[WARN]: FAILSAFED RC Channels: <");
    } else {
      Serial.print("[INFO]: RC Channels: <");
    }

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

  /*
   * Channel 1: Speed
   * Channel 2: Direction
   * Channel 5: Armed
   * Channel 6: Control Source
   * Channel 7: Wifi
  */

  // TODO is this range actually 988 to 2012 as we're seeing on the controller?
  // float direction = mapRange(992, 2008, -1, 1, crsf->rcToUs(rcData->value[1]));
  StorageState &state = Storage::getState();
  float speed = mapRange(988, 2012, -state.motorSpeedLimit, state.motorSpeedLimit, TICKS_TO_US(channels[CHANNEL_SPEED]));
  float direction = mapRange(988, 2012, -1, 1, TICKS_TO_US(channels[1]));
  // if within 10 of 1500, set speed to 0
  if (abs(TICKS_TO_US(channels[CHANNEL_SPEED]) - 1500) < 10) {
    speed = 0;
  }
  if (abs(TICKS_TO_US(channels[CHANNEL_DIRECTION]) - 1500) < 10) {
    direction = 0;
  }
  if (lastSpeed !=  speed || lastDirection != direction) {
    lastSpeed = speed;
    lastDirection = direction;
    taskMessage = {
            .type = DataManager::Type::SET_MOTOR_SPEED_COMBINED,
            .as = {.motorSpeedCombined = {
              .speed = speed, 
              .direction = direction,
              .controlSource = DataManager::RobotControlSource::MANUAL,
              }},
    };
    DataManager::receiveMessage(taskMessage);
  }

  // Check armed switch
  const int armedChannel = TICKS_TO_US(channels[CHANNEL_ARM]);
  const int controlSourceChannel = TICKS_TO_US(channels[CHANNEL_CONTROL_SOURCE]);
  const int wifiChannel = TICKS_TO_US(channels[CHANNEL_WIFI]);

  const bool isArmedSwitch = armedChannel < 1500;
  // 1000 = manual, 1500 = obc, 2000 = flight controller
  const DataManager::RobotControlSource controlSourceSwitch = 
    controlSourceChannel < 1250 
      ? DataManager::RobotControlSource::MANUAL 
      : controlSourceChannel < 1750 ? DataManager::RobotControlSource::ONBOARD_COMPUTER : DataManager::RobotControlSource::FLIGHT_CONTROLLER;
  const bool isWifiSwitch = wifiChannel > 1500;
  
  DataManager::Message taskMessage;
  if (armedSwitch != isArmedSwitch) {
    armedSwitch = isArmedSwitch;
    taskMessage.type = DataManager::Type::TX_SWITCH_ARMED;
    taskMessage.as.txBinarySwitch = isArmedSwitch;
    DataManager::receiveMessage(taskMessage);
  }

  if (CFG_ENABLE_EXTRA_SWITCHES) {
    if (controlSource != controlSourceSwitch) {
      controlSource = controlSourceSwitch;
      taskMessage.type = DataManager::Type::TX_SWITCH_CONTROL_SOURCE;
      taskMessage.as.txControlSourceSwitch = controlSourceSwitch;
      DataManager::receiveMessage(taskMessage);
    }
  }

  if (wifiSwitch != isWifiSwitch || firstRcChannels) {
    wifiSwitch = isWifiSwitch;
    Serial.println("WIFI switch changed");
    taskMessage.type = DataManager::Type::TX_SWITCH_WEB_SERVER;
    taskMessage.as.txBinarySwitch = isWifiSwitch;
    DataManager::receiveMessage(taskMessage);
  }

  firstRcChannels = false;

}

void TaskRC::onFailsafeActivated() {
  Serial.println("[WARN]: Failsafe detected.");
  taskMessage = {.type = DataManager::Type::TX_LOST};
  DataManager::receiveMessage(taskMessage);
}

void TaskRC::onFailsafeCleared() { 
  if (isFailsafeActive) {
    Serial.println("[WARN] attempted to clear failsafe but RC failsafe is still active");
    return;
  }
  if (receiverTimedOut) {
    Serial.println("[WARN] attempted to clear failsafe but receiver is still timed out");
    return;
  }
  Serial.println("[INFO]: Failsafe cleared.");
  taskMessage = {.type = DataManager::Type::TX_RESTORED};
  DataManager::receiveMessage(taskMessage);
}

void TaskRC::onFailsafe(const bool failsafe) {
  isFailsafeActive = failsafe;
  if (failsafe) {
    onFailsafeActivated();
  } else {
    onFailsafeCleared();
  }
}