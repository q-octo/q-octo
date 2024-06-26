/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#pragma once

#include "task_data_manager.h"
#include "crsf.h"
#include "storage.h"

class TaskRC {
public:
  typedef enum {
    BATTERY,
    STATE_UPDATE,
  } MessageType;

  typedef struct {
    MessageType type;
    union {
      DataManager::Battery battery;
      DataManager::State state;
    } as;
  } Message;

  static void init();

  static void loop();

  static void receiveMessage(const Message &message);


private:

  /* RC Channels Event Callback. */
  static void onLinkStatisticsUpdate(link_statistics_t linkStats);

  static void onReceiveChannels(const uint16_t channels[16]);

  static void onFailsafe(bool failsafe);

  static void onFailsafeActivated();

  static void onFailsafeCleared();
  static bool failsafeActive() { return isFailsafeActive || receiverTimedOut;}
  static void sendStateAsTelem(const DataManager::State &state);


  static inline DataManager::Message taskMessage;
  /* A flag to hold the fail-safe status. */
  static inline bool isFailsafeActive = false;
  /// True if we haven't received a link stats update in the expected timeframe.
  static inline bool receiverTimedOut = false;
  static inline uint32_t lastReceiverUpdateMs = 0;

  /* RC Channels data. */
  static inline int rcChannelCount = 8;
  // 16 channels
  static inline const char *rcChannelNames[] = {
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

  static inline uint32_t lastRcChannelsLogMs = 0;
  static inline uint32_t lastRcLinkStatsLogMs = 0;
  static inline uint32_t lastBroadcastMs = 0;
  static inline float lastSpeed = 0;
  static inline float lastDirection = 0;
  static inline link_statistics_t lastLinkStats = {0};
  static inline uint16_t lastChannels[16] = {0};
  static inline uint16_t lastChannelsUs[16] = {0};
  static inline bool armedSwitch = false;
  static inline bool wifiSwitch = false;
  static inline DataManager::RobotControlSource controlSource = DataManager::MANUAL;
  static inline bool firstRcChannels = true;
  static inline crsf_instance receiverCrsfIns;

  static inline StorageState &state = Storage::getState();
  
  // 0-15
  static const uint8_t CHANNEL_SPEED = 0;
  static const uint8_t CHANNEL_DIRECTION = 1;
  static const uint8_t CHANNEL_ARM = 4;
  static const uint8_t CHANNEL_CONTROL_SOURCE = 5;
  static const uint8_t CHANNEL_WIFI = 6;

  static void setThresholds();


};
