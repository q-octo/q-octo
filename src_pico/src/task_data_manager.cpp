#include "task_data_manager.h"
#include "companion.h"
#include "task_motors.h"
#include "task_power_monitor.h"
#include "task_rc.h"

QueueHandle_t dataManagerQueue;

static TaskMessage::State state;
static Companion::Message companionMessage;

/*
This task will have a higher priority than the tasks that message it.
When the sending task sends a message to the queue, this task will preempt the
task and process the message.
This prevents the queue from overflowing.

Likewise, tasks that we forward messages to should have a higher priority than
this task so that their queue does not overflow.
*/

void broadcastStateUpdate() {
  companionMessage = {
      .type = Companion::MessageType::STATE_UPDATE,
      .as = {.state = state},
  };
  xQueueSend(Companion::companionQueue, &companionMessage, 0);
}

void taskDataManager(void *pvParameters) {
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskDataManager started");
  dataManagerQueue = xQueueCreate(10, sizeof(TaskMessage::Message));
  if (dataManagerQueue == nullptr) {
    Serial.println("Failed to create dataManagerQueue");
    vTaskDelete(nullptr);
  }

  static TaskMessage::Message message;
  static TaskRC::Message rcMessage;
  static TaskControlMotors::Message controlMotorsMessage;
  static TaskPowerMonitor::Message powerMonitorMessage;

  for (;;) {
    if (xQueueReceive(dataManagerQueue, &message, portMAX_DELAY)) {
      switch (message.type) {
      case TaskMessage::Type::STATE_MOTORS:
        state.motors = message.as.motors;
        broadcastStateUpdate();
        break;
      case TaskMessage::Type::STATE_RC:
        state.rc = message.as.rc;
        broadcastStateUpdate();
        break;
      case TaskMessage::Type::ENABLE_WEB_SERVER:
        state.webServerEnabled = true;
        companionMessage = {.type = Companion::MessageType::ENABLE_WEB_SERVER};
        xQueueSend(Companion::companionQueue, &companionMessage, 0);
        broadcastStateUpdate();
        break;
      case TaskMessage::Type::DISABLE_WEB_SERVER:
        state.webServerEnabled = false;
        companionMessage = {.type = Companion::MessageType::ENABLE_WEB_SERVER};
        xQueueSend(Companion::companionQueue, &companionMessage, 0);
      case TaskMessage::Type::ENABLE_MOTORS:
        break;
      case TaskMessage::Type::DISABLE_MOTORS:
        break;
        broadcastStateUpdate();
      case TaskMessage::Type::SET_MOTOR_SPEED_COMBINED:
        controlMotorsMessage = {
            .type = TaskControlMotors::MessageType::SET_SPEED_COMBINED,
            .as = {.speedCombined = message.as.motorSpeedCombined},
        };
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        break;
      case TaskMessage::Type::SET_MOTOR_SPEED_INDIVIDUAL:
        controlMotorsMessage = {
            .type = TaskControlMotors::MessageType::SET_SPEED_INDIVIDUAL,
            .as = {.speedIndividual = message.as.motorSpeedIndividual},
        };
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        break;
      case TaskMessage::Type::TX_LOST:
        controlMotorsMessage = {.type =
                                    TaskControlMotors::MessageType::DISABLE};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        // TODO update display status to NO_TX
        break;
      case TaskMessage::Type::TX_RESTORED:
        controlMotorsMessage = {.type = TaskControlMotors::MessageType::ENABLE};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        // Update display status
        break;
      case TaskMessage::CAN_MESSAGE_MOTOR_L:
        controlMotorsMessage = {
            .type = TaskControlMotors::MessageType::CAN_MESSAGE_MOTOR_L,
            .as = {.canMessage = message.as.canMessage},
        };
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
      case TaskMessage::CAN_MESSAGE_MOTOR_R:
        controlMotorsMessage = {
            .type = TaskControlMotors::MessageType::CAN_MESSAGE_MOTOR_R,
            .as = {.canMessage = message.as.canMessage},
        };
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        break;
      case TaskMessage::CAN_MESSAGE_ROT_ENC_L:
      case TaskMessage::CAN_MESSAGE_ROT_ENC_R:
        break;
      case TaskMessage::CAN_MESSAGE_POWER_MONITOR:
        powerMonitorMessage = {
            .type = TaskPowerMonitor::MessageType::CAN_MESSAGE,
            .as = {.canMessage = message.as.canMessage},
        };
        xQueueSend(powerMonitorQueue, &powerMonitorMessage, 0);
        break;
      case TaskMessage::Type::BATT_OK:
        state.battery = message.as.battery;
        rcMessage.type = TaskRC::MessageType::BATTERY;
        rcMessage.as.battery = message.as.battery;
        xQueueSend(rcSendQueue, &rcMessage, 0);
        broadcastStateUpdate();
        break;
      case TaskMessage::Type::BATT_VOLTAGE_LOW:
        // Disable motors
        // TODO Update status on web dashboard & display
        controlMotorsMessage = {.type =
                                    TaskControlMotors::MessageType::DISABLE};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
      case TaskMessage::Type::BATT_VOLTAGE_CRITICAL:
        controlMotorsMessage = {.type =
                                    TaskControlMotors::MessageType::DISABLE};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        exit(1);
        break;
      case TaskMessage::SET_LOW_VOLTAGE_THRESHOLD:
        state.lowVoltageThreshold = message.as.voltageThreshold;
        powerMonitorMessage = {
            .type = TaskPowerMonitor::MessageType::SET_LOW_VOLTAGE_THRESHOLD,
            .as = {.voltageThreshold = message.as.voltageThreshold},
        };
        xQueueSend(powerMonitorQueue, &powerMonitorMessage, 0);
        broadcastStateUpdate();
        break;
      case TaskMessage::SET_CRITICAL_VOLTAGE_THRESHOLD:
        state.criticalVoltageThreshold = message.as.voltageThreshold;
        powerMonitorMessage = {
            .type =
                TaskPowerMonitor::MessageType::SET_CRITICAL_VOLTAGE_THRESHOLD,
            .as = {.voltageThreshold = message.as.voltageThreshold},
        };
        xQueueSend(powerMonitorQueue, &powerMonitorMessage, 0);
        broadcastStateUpdate();
        break;
      case TaskMessage::SET_BATTERY_COUNT:
        state.batteryCount = message.as.batteryCount;
        powerMonitorMessage = {
            .type = TaskPowerMonitor::MessageType::SET_BATTERY_COUNT,
            .as = {.batteryCount = message.as.batteryCount},
        };
        xQueueSend(powerMonitorQueue, &powerMonitorMessage, 0);
        broadcastStateUpdate();
        break;
      case TaskMessage::FOLD_WHEELS:
        controlMotorsMessage = {
            .type = TaskControlMotors::MessageType::FOLD_WHEELS};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        break;
      default:
        Serial.println("[ERROR] Unknown message type");
      }
    }
  }
}