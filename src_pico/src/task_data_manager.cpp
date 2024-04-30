#include "task_data_manager.h"

#include "task_motors.h"
#include "task_power_monitor.h"
#include "task_rc.h"
#include "companion.h"
#include "computer.h"

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
DataManager::State state{};
Companion::Message companionMessage{};
Computer::Message computerMessage{};
TaskRC::Message rcMessage{};

void DataManager::broadcastStateUpdate() {
  companionMessage.type = Companion::MessageType::STATE_UPDATE;
  companionMessage.as = {.state = state};
  Companion::receiveMessage(companionMessage);
  computerMessage.type = Computer::MessageType::STATE_UPDATE;
  computerMessage.as = {.state = state};
  Computer::receiveMessage(computerMessage);
  rcMessage.type = TaskRC::MessageType::STATE_UPDATE;
  rcMessage.as = {.state = state};
  TaskRC::receiveMessage(rcMessage);
}

void DataManager::setWebServerEnabled(bool enabled) {
  state.webServerEnabled = enabled;
  companionMessage.type = enabled ? Companion::MessageType::ENABLE_WEB_SERVER
                                  : Companion::MessageType::DISABLE_WEB_SERVER;
  Companion::receiveMessage(companionMessage);
  broadcastStateUpdate();
}

void DataManager::receiveMessage(const DataManager::Message &message) {
  static TaskControlMotors::Message controlMotorsMessage;
  static TaskPowerMonitor::Message powerMonitorMessage;

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
    case TOGGLE_WEB_SERVER_ENABLED:
      // plz don't deadlock :)
      setWebServerEnabled(!state.webServerEnabled);
      break;
    case ENABLE_MOTORS:
      break;
    case DISABLE_MOTORS:
      break;
    case SET_MOTOR_SPEED_COMBINED:
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
      // TODO update display status to NO_TX
      break;
    case TX_RESTORED:
      controlMotorsMessage.type = TaskControlMotors::MessageType::ENABLE;
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      // Update display status
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
      rcMessage.type = TaskRC::MessageType::BATTERY;
      rcMessage.as.battery = message.as.battery;
      TaskRC::receiveMessage(rcMessage);
      broadcastStateUpdate();
      break;
    case BATT_VOLTAGE_LOW:
      // Disable motors
      // TODO Update status on web dashboard & display
      controlMotorsMessage.type = TaskControlMotors::MessageType::DISABLE;
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case BATT_VOLTAGE_CRITICAL:
      controlMotorsMessage.type = TaskControlMotors::MessageType::DISABLE;
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      exit(1);
      break;
    case FOLD_WHEELS:
      controlMotorsMessage.type = TaskControlMotors::MessageType::FOLD_WHEELS;
      TaskControlMotors::receiveMessage(controlMotorsMessage);
      break;
    case DISPLAY_BUTTON_PRESSED:
      computerMessage.type = Computer::MessageType::DISPLAY_BUTTON;
      computerMessage.as = {.displayButton = message.as.displayButton};
      Computer::receiveMessage(computerMessage);
      break;
    case STORAGE_UPDATE:
      broadcastStateUpdate();
      break;
    case DISPLAY_MESSAGES:
      state.displayMessages = message.as.displayMessages;
      broadcastStateUpdate();
    default:
      Serial.println("[ERROR] Unknown message type");
  }
}
