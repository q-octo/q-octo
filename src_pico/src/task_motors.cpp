#include "task_motors.h"
#include "Arduino.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task_control_motors.h"

#define STATUS_REQUEST_FREQUENCY 500   // ms
#define STATUS_BROADCAST_FREQUENCY 500 // ms
#define TASK_FREQUENCY 500             // ms

uint32_t lastStatusRequestMs = 0;
uint32_t lastStatusBroadcastMs = 0;

void taskMotors(void *pvParameters) {
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskMotors started");

  for (;;) {
    // send a request to the cybergear to receive motor status (position, speed,
    // torque, temperature)
    const uint32_t currentMillis = millis();
    if (currentMillis - lastStatusRequestMs >= STATUS_REQUEST_FREQUENCY) {
      lastStatusRequestMs = currentMillis;
      cybergearL.request_status();
      cybergearR.request_status();
    }

    if (currentMillis - lastStatusBroadcastMs >= STATUS_BROADCAST_FREQUENCY) {
      lastStatusBroadcastMs = currentMillis;
      auto statusL = cybergearL.get_status();
      auto statusR = cybergearR.get_status();
      TaskMessage::Motors motors = {
          .left =
              {
                  .temperature = statusL.temperature,
                  .RPM = statusL.speed,
                  .torque = statusL.torque,
                  .position = statusL.position,
              },
          .right =
              {
//                  .temperature = statusR.temperature,
                  .temperature = static_cast<uint16_t>((currentMillis % 24)),
                  .RPM = statusR.speed,
                  .torque = statusR.torque,
                  .position = statusR.position,
              },
      };
      TaskMessage::Message message = {.type = TaskMessage::Type::STATE_MOTORS,
                             .motors = motors};
      xQueueSend(dataManagerQueue, &message, 0);
    }

    // When the motor CAN data was monitored via a saleae logic analyzer, it was
    // sending a constant stream of messages. Do we receive these messages, are
    // they status updates? If so, we could avoid this polling approach

    delay(pdMS_TO_TICKS(TASK_FREQUENCY));
  }
}
