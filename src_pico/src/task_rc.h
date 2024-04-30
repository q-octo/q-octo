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


  static inline DataManager::Message taskMessage;
  /* A flag to hold the fail-safe status. */
  static inline bool isFailsafeActive = false;

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
  static inline float lastRPM = 0;
  static inline link_statistics_t lastLinkStats = {0};
  static inline uint16_t lastChannels[16] = {0};

  static inline Storage::State &state = Storage::getState();

  static void setThresholds();


};
