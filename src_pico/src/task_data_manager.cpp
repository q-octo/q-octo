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
#include "task_data_manager.h"

#include "task_motors.h"
#include "task_power_monitor.h"
#include "task_rc.h"
#include "companion.h"
#include "computer.h"
#include "system_status.h"
#include "enable_wifi.h"
#include "enable_motors.h"

/*
This task will have a higher priority than the tasks that message it.
When the sending task sends a message to the queue, this task will preempt the
task and process the message.
This prevents the queue from overflowing.

Likewise, tasks that we forward messages to should have a higher priority than
this task so that their queue does not overflow.
*/


// These remain as global variables rather than static inline class variables because this would cause circular imports
// in the header files.
Companion::Message companionMessage{};
Computer::Message computerMessage{};
TaskRC::Message rcMessage{};
TaskControlMotors::Message controlMotorsMessage{};

void DataManager::broadcastStateUpdate() {
  companionMessage = {.type = Companion::MessageType::STATE_UPDATE, .as = {.state = state}};
  Companion::receiveMessage(companionMessage);
  computerMessage = {.type = Computer::MessageType::STATE_UPDATE, .as = {.state = state}};
  Computer::receiveMessage(computerMessage);
  rcMessage = {.type = TaskRC::MessageType::STATE_UPDATE, .as = {.state = state}};
  TaskRC::receiveMessage(rcMessage);
  controlMotorsMessage = {.type = TaskControlMotors::MessageType::STATE_UPDATE, .as = {.state = state}}; 
  TaskControlMotors::receiveMessage(controlMotorsMessage);
}

void DataManager::setWebServerEnabled(bool enabled) {
  state.webServerEnabled = enabled; 
  broadcastStateUpdate();
}

void DataManager::receiveMessage(const DataManager::Message &message) {
  static TaskPowerMonitor::Message powerMonitorMessage;

  SystemStatus::receiveDataManagerMessage(message);

  switch (message.type) {
    case STATE_MOTORS:
      state.motors = message.as.motors;
      broadcastStateUpdate(); 
      break;
    case STATE_RC:
      state.rc = message.as.rc;
      broadcastStateUpdate();
      break;
    case ENABLE_WEB_SERVER:
      setWebServerEnabled(true);
      break;
    case DISABLE_WEB_SERVER:
      setWebServerEnabled(false);
      break;
    case DISPLAY_WEB_SERVER_BTN_PRESSED:
      EnableWifi::onWifiButtonPress();
      break;
    case ENABLE_MOTORS:
      controlMotorsMessage.type = TaskControlMotors::MessageType::ENABLE;
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case DISABLE_MOTORS:
      controlMotorsMessage.type = TaskControlMotors::MessageType::DISABLE;
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case SET_MOTOR_SPEED_COMBINED:
      if (message.as.motorSpeedCombined.controlSource != state.controlSource) {
        // Ignore a control message from an inactive source
        break;
      } 
      controlMotorsMessage.type = TaskControlMotors::MessageType::SET_SPEED_COMBINED;
      controlMotorsMessage.as = {.speedCombined = message.as.motorSpeedCombined};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case SET_MOTOR_SPEED_INDIVIDUAL:
      controlMotorsMessage.type = TaskControlMotors::MessageType::SET_SPEED_INDIVIDUAL;
      controlMotorsMessage.as = {.speedIndividual = message.as.motorSpeedIndividual};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case TX_LOST:
      controlMotorsMessage.type = TaskControlMotors::MessageType::DISABLE;
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      EnableWifi::onTxFailsafed();
      EnableMotors::onTxFailsafed();
      broadcastStateUpdate();
      break;
    case TX_RESTORED:
      EnableWifi::onTxFailsafeCleared();
      EnableMotors::onTxFailsafeCleared();
      broadcastStateUpdate();
      break;
    case CAN_MESSAGE_MOTOR_L:
      controlMotorsMessage.type = TaskControlMotors::MessageType::CAN_MESSAGE_MOTOR_L;
      controlMotorsMessage.as = {.canMessage = message.as.canMessage};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case CAN_MESSAGE_MOTOR_R:
      controlMotorsMessage.type = TaskControlMotors::MessageType::CAN_MESSAGE_MOTOR_R;
      controlMotorsMessage.as = {.canMessage = message.as.canMessage};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case CAN_MESSAGE_ROT_ENC_L:
    case CAN_MESSAGE_ROT_ENC_R:
      break;
    case CAN_MESSAGE_POWER_MONITOR:
      powerMonitorMessage.type = TaskPowerMonitor::MessageType::CAN_MESSAGE;
      powerMonitorMessage.as = {.canMessage = message.as.canMessage};
      TaskPowerMonitor::receiveMessage(powerMonitorMessage);
      break;
    case BATT_OK:
      state.battery = message.as.battery;
      EnableMotors::onBatteryVoltage(false);
      rcMessage.type = TaskRC::MessageType::BATTERY;
      rcMessage.as.battery = message.as.battery;
      TaskRC::receiveMessage(rcMessage);
      broadcastStateUpdate();
      break;
    case BATT_VOLTAGE_LOW:
      // Disable motors
      EnableMotors::onBatteryVoltage(true);
      broadcastStateUpdate();
      break;
    case BATT_VOLTAGE_CRITICAL:
      EnableMotors::onBatteryVoltage(true);
      exit(1);
      break;
    case FOLD_WHEELS:
      controlMotorsMessage.type = TaskControlMotors::MessageType::FOLD_WHEELS;
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case DISPLAY_BUTTON_PRESSED:
      EnableWifi::onWifiButtonPress();
      computerMessage.type = Computer::MessageType::DISPLAY_BUTTON;
      computerMessage.as = {.displayButton = message.as.displayButton};
      Computer::receiveMessage(computerMessage);
      break;
    case BUTTON_DOWN:
      if (message.as.physicalButton == PhysicalButton::WEB_SERVER) {
        EnableWifi::onPhysicalSwitchChange(true);
      } else if (message.as.physicalButton == PhysicalButton::MOTORS) {
        // TODO implement
      }
      break;
    case BUTTON_UP:
      if (message.as.physicalButton == PhysicalButton::WEB_SERVER) {
        EnableWifi::onPhysicalSwitchChange(false);
      } else if (message.as.physicalButton == PhysicalButton::MOTORS) {
        // TODO implement
      }
      break;
    case STORAGE_UPDATE:
      broadcastStateUpdate();
      break;
    case DISPLAY_MESSAGES:
      state.displayMessages = message.as.displayMessages;
      broadcastStateUpdate();
    case UPDATE_MOTOR_SPEED_LIMIT:
      controlMotorsMessage.type = TaskControlMotors::MessageType::UPDATE_SPEED_LIMIT;
      controlMotorsMessage.as = {.floatMotorParam = message.as.floatMotorParam};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case UPDATE_MOTOR_CURRENT_LIMIT:
      controlMotorsMessage.type = TaskControlMotors::MessageType::UPDATE_CURRENT_LIMIT;
      controlMotorsMessage.as = {.floatMotorParam = message.as.floatMotorParam};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case UPDATE_MOTOR_TORQUE_LIMIT:
      controlMotorsMessage.type = TaskControlMotors::MessageType::UPDATE_TORQUE_LIMIT;
      controlMotorsMessage.as = {.floatMotorParam = message.as.floatMotorParam};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case UPDATE_MOTOR_SPEED_KP:
      controlMotorsMessage.type = TaskControlMotors::MessageType::UPDATE_SPEED_KP;
      controlMotorsMessage.as = {.floatMotorParam = message.as.floatMotorParam};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case UPDATE_MOTOR_SPEED_KI:
      controlMotorsMessage.type = TaskControlMotors::MessageType::UPDATE_SPEED_KI;
      controlMotorsMessage.as = {.floatMotorParam = message.as.floatMotorParam};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case LEFT_MOTOR_PARAM_UPDATED:
      // Serial.println("Left motor param updated");
      state.leftMotorLimits = message.as.motorParams;
      broadcastStateUpdate();
      break;
    case RIGHT_MOTOR_PARAM_UPDATED:
      // Serial.println("Right motor param updated");
      state.rightMotorLimits = message.as.motorParams;
      broadcastStateUpdate();
      break;
    case TX_SWITCH_ARMED:
      EnableMotors::onTxSwitchChange(message.as.txBinarySwitch);
      state.armed = message.as.txBinarySwitch;
      broadcastStateUpdate();
      break;
    case TX_SWITCH_CONTROL_SOURCE: 
      state.controlSource = message.as.txControlSourceSwitch;
      broadcastStateUpdate();
      // Stop motors
      controlMotorsMessage.type = TaskControlMotors::MessageType::SET_SPEED_INDIVIDUAL;
      controlMotorsMessage.as = {.speedIndividual = {
        .leftSpeed = 0,
        .rightSpeed = 0,
        .controlSource = state.controlSource
      }};
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case TX_SWITCH_WEB_SERVER:
      EnableWifi::onTxSwitchChange(message.as.txBinarySwitch);
      break;
    default:
      Serial.println("[ERROR] Unknown message type");
  }
}
