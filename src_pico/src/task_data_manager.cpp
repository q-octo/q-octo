#include "task_data_manager.h"
#include "task_display.h"
#include "task_rc.h"
#include "task_motors.h"
#include "web_server.h"
#include "task_power_monitor.h"

QueueHandle_t dataManagerQueue;

/*
This task will have a higher priority than the tasks that message it.
When the sending task sends a message to the queue, this task will preempt the
task and process the message.
This prevents the queue from overflowing.

Likewise, tasks that we forward messages to should have a higher priority than
this task so that their queue does not overflow.
*/

void taskDataManager(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskDataManager started");
  dataManagerQueue = xQueueCreate(10, sizeof(TaskMessage::Message));
  if (dataManagerQueue == nullptr)
  {
    Serial.println("Failed to create dataManagerQueue");
    vTaskDelete(nullptr);
  }

  static TaskMessage::Message message;
  static TaskDisplay::Message displayMessage;
  static TaskRC::Message rcMessage;
  static TaskControlMotors::Message controlMotorsMessage;
  static WSWebServer::Message webServerMessage;
  static TaskPowerMonitor::Message powerMonitorMessage;

  for (;;)
  {
    if (xQueueReceive(dataManagerQueue, &message, portMAX_DELAY))
    {
      switch (message.type)
      {
      case TaskMessage::Type::STATE_MOTORS:
        displayMessage = {
            .type = TaskDisplay::MessageType::MOTORS,
            .as = {.motors = message.as.motors},
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        // Can send the message to other tasks such as the web server
        // task in the future
        break;
      case TaskMessage::Type::BATT_OK:
        displayMessage = {
            .type = TaskDisplay::MessageType::BATTERY,
            .as = {.battery = message.as.battery},
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        rcMessage = {
            .type = TaskRC::MessageType::BATTERY,
            .as = {.battery = message.as.battery},
        };
        xQueueSend(rcSendQueue, &rcMessage, 0);
        break;
      case TaskMessage::Type::BATT_VOLTAGE_LOW:
        // Disable motors
        // TODO Update status on web dashboard & display
        controlMotorsMessage = {.type = TaskControlMotors::MessageType::DISABLE};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
      case TaskMessage::Type::BATT_VOLTAGE_CRITICAL:
        controlMotorsMessage = {.type = TaskControlMotors::MessageType::DISABLE};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        exit(1);
        break;
      case TaskMessage::Type::STATE_RC:
        displayMessage = {
            .type = TaskDisplay::MessageType::RC,
            .as = {.rc = message.as.rc},
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        break;
      case TaskMessage::Type::STATE_DIAGNOSTICS:
        displayMessage = {
            .type = TaskDisplay::MessageType::DIAGNOSTICS,
            .as = {.diagnostics = message.as.diagnostics},
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        break;
      case TaskMessage::Type::ENABLE_WEB_SERVER:
        displayMessage = {
            .type = TaskDisplay::MessageType::DIAGNOSTICS,
            .as = {.diagnostics = {.diagnosticsMode = true}},
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        // webServerMessage = {.type = WSWebServer::MessageType::ENABLE};
        // xQueueSend(webServerQueue, &webServerMessage, 0);
        break;
      case TaskMessage::Type::DISABLE_WEB_SERVER:
        displayMessage = {
            .type = TaskDisplay::MessageType::DIAGNOSTICS,
            .as = {.diagnostics = {.diagnosticsMode = false}},
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        // webServerMessage = {.type = WSWebServer::MessageType::DISABLE};
        // xQueueSend(webServerQueue, &webServerMessage, 0);
      case TaskMessage::Type::ENABLE_MOTORS:
        break;
      case TaskMessage::Type::DISABLE_MOTORS:
        break;
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
        controlMotorsMessage = {.type = TaskControlMotors::MessageType::DISABLE};
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
      default:
        Serial.println("[ERROR] Unknown message type");
      }
    }
  }
}