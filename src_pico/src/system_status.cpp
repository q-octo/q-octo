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
    case DataManager::DISPLAY_WEB_SERVER_BTN_PRESSED:
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
