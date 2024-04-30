#include "system_status.h"

DataManager::Status SystemStatus::getStatus() {
  if (lowBattery) {
    return DataManager::LOW_BATTERY;
  } else if (noTxSignal) {
    return DataManager::NO_TX_SIGNAL;
  } else if (motorError) {
    return DataManager::MOTOR_ERROR;
  } else if (disarmed) {
    return DataManager::DISARMED;
  } else {
    return DataManager::OK;
  }
}

void SystemStatus::receiveDataManagerMessage(const DataManager::Message &message) {
  switch (message.type) {
    case DataManager::TX_LOST:
      noTxSignal = true;
      break;
    case DataManager::TX_RESTORED:
      noTxSignal = false;
      break;
    case DataManager::BATT_OK:
      lowBattery = false;
      break;
    case DataManager::BATT_VOLTAGE_LOW:
    case DataManager::BATT_VOLTAGE_CRITICAL:
      lowBattery = true;
      break;
    case DataManager::STATE_MOTORS:
    case DataManager::STATE_RC:
    case DataManager::ENABLE_WEB_SERVER:
    case DataManager::DISABLE_WEB_SERVER:
    case DataManager::TOGGLE_WEB_SERVER_ENABLED:
    case DataManager::ENABLE_MOTORS:
    case DataManager::DISABLE_MOTORS:
    case DataManager::SET_MOTOR_SPEED_COMBINED:
    case DataManager::SET_MOTOR_SPEED_INDIVIDUAL:
    case DataManager::CAN_MESSAGE_MOTOR_L:
    case DataManager::CAN_MESSAGE_MOTOR_R:
    case DataManager::CAN_MESSAGE_ROT_ENC_L:
    case DataManager::CAN_MESSAGE_ROT_ENC_R:
    case DataManager::CAN_MESSAGE_POWER_MONITOR:
    case DataManager::FOLD_WHEELS:
    case DataManager::DISPLAY_BUTTON_PRESSED:
    case DataManager::STORAGE_UPDATE:
    case DataManager::DISPLAY_MESSAGES:
    case DataManager::BUTTON_DOWN:
    case DataManager::BUTTON_UP:
      break;
  }
}
