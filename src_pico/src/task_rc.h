#pragma once

#include "task_data_manager.h"
#include "crsf.h"

namespace TaskRC
{
  // public:
  typedef enum
  {
    BATTERY,
  } MessageType;

  typedef struct
  {
    MessageType type;
    union
    {
      DataManager::Battery battery;
    } as;
  } Message;

  void init();

  void loop();

  void receiveMessage(const Message &message);

  // private:

  /* RC Channels Event Callback. */
  void onLinkStatisticsUpdate(link_statistics_t linkStats);

  void onReceiveChannels(const uint16_t channels[16]);

  void onFailsafe(bool failsafe);
  void onFailsafeActivated();
  void onFailsafeCleared();

  

};
