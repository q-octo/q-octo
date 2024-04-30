#pragma once

#include "task_data_manager.h"

class SystemStatus {
public:


  static DataManager::Status getStatus();

  static void receiveDataManagerMessage(const DataManager::Message &message);

private:
  static inline bool lowBattery;
  static inline bool noTxSignal;
  // TODO set these fields
  static inline bool motorError;
  static inline bool disarmed;
};
