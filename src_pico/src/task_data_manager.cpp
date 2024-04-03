#include "task_data_manager.h"
#include "task_display.h"
#include "task_rc.h"
#include "task_control_motors.h"

QueueHandle_t dataManagerQueue;

TaskDisplayMessage displayMessage;
TaskRCMessage rcMessage;
TaskControlMotors::Message controlMotorsMessage;

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
  dataManagerQueue = xQueueCreate(10, sizeof(TaskMessage));
  if (dataManagerQueue == nullptr)
  {
    Serial.println("Failed to create dataManagerQueue");
    vTaskDelete(nullptr);
  }

  TaskMessage message;
  for (;;)
  {
    if (xQueueReceive(dataManagerQueue, &message, portMAX_DELAY))
    {
      switch (message.type)
      {
      case TaskMessageType::STATE_MOTORS:
        Serial.println("taskDataManager: STATE_MOTORS received");
        displayMessage = {
            .type = TaskDisplay::MessageType::MOTORS,
            .motors = message.motors,
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        // Can send the message to other tasks such as the web server
        // task in the future
        break;
      case TaskMessageType::STATE_BATTERY:
        displayMessage = {
            .type = TaskDisplay::MessageType::BATTERY,
            .battery = message.battery,
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        rcMessage = {
            .type = TaskRC::MessageType::BATTERY,
            .as = {.battery = message.battery},
        };
        xQueueSend(rcSendQueue, &rcMessage, 0);
        break;
      case TaskMessageType::STATE_RC:
        displayMessage = {
            .type = TaskDisplay::MessageType::RC,
            .rc = message.rc,
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        break;
      case TaskMessageType::STATE_DIAGNOSTICS:
        displayMessage = {
            .type = TaskDisplay::MessageType::DIAGNOSTICS,
            .diagnostics = message.diagnostics,
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        break;
      case TaskMessageType::ENABLE_DIAGNOSTICS:
        // TODO(niall) enable web server
        // For now, just reply immediately
        displayMessage = {
            .type = TaskDisplay::MessageType::DIAGNOSTICS,
            .diagnostics =
                {
                    .diagnosticsMode = true,
                },
        };
        xQueueSend(displayQueue, &displayMessage, 0);
        break;
      case TaskMessageType::DISABLE_DIAGNOSTICS:
        // TODO(niall) disable web server
        // For now, just reply immediately
        displayMessage = {
            .type = TaskDisplay::MessageType::DIAGNOSTICS,
            .diagnostics =
                {
                    .diagnosticsMode = false,
                },
        };
        xQueueSend(displayQueue, &displayMessage, 0);
      case TaskMessageType::ENABLE_MOTORS:
        break;
      case TaskMessageType::DISABLE_MOTORS:
        break;
      case TaskMessageType::SET_MOTOR_SPEED_COMBINED:
        controlMotorsMessage = {
            .type = TaskControlMotors::MessageType::SET_SPEED_COMBINED,
            .as = {.speedCombined = message.motorSpeedCombined},
        };
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        break;
      case TaskMessageType::SET_MOTOR_SPEED_INDIVIDUAL:
        controlMotorsMessage = {
            .type = TaskControlMotors::MessageType::SET_SPEED_INDIVIDUAL,
            .as = {.speedIndividual = message.motorSpeedIndividual},
        };
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        break;
      case TaskMessageType::TX_LOST:
        controlMotorsMessage = {.type = TaskControlMotors::MessageType::DISABLE};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        // TODO update display status to NO_TX
        break;
      case TaskMessageType::TX_RESTORED:
        controlMotorsMessage = {.type = TaskControlMotors::MessageType::ENABLE};
        xQueueSend(controlMotorsQueue, &controlMotorsMessage, 0);
        // Update display status
        break;
      default:
        Serial.println("[ERROR] Unknown message type");
      }
    }
  }
}