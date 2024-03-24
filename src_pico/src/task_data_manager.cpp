#include "task_data_manager.h"
#include "task_display.h"

QueueHandle_t dataManagerQueue;

/*
This task will have a higher priority than the tasks that message it.
When the sending task sends a message to the queue, this task will preempt the
task and process the message.
This prevents the queue from overflowing.

Likewise, tasks that we forward messages to should have a higher priority than
this task so that their queue does not overflow.
*/

void taskDataManager(void *pvParameters) {
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskDataManager started");
  dataManagerQueue = xQueueCreate(10, sizeof(TaskMessage));
  if (dataManagerQueue == nullptr) {
    Serial.println("Failed to create dataManagerQueue");
    vTaskDelete(nullptr);
  }

  TaskMessage message;
  for (;;) {
    if (xQueueReceive(dataManagerQueue, &message, portMAX_DELAY)) {
      TaskDisplayMessage displayMessage;
      switch (message.type) {
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

      default:
        Serial.println("[ERROR] Unknown message type");
      }
    }
  }
}