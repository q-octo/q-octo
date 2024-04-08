#include "task_can.h"
#include "can.h"
#include "task_motors.h"
#include "task_data_manager.h"

#include "FreeRTOS.h"
#include "queue.h"

void taskCAN(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskCAN started");
  for (;;)
  {
    CanCommunication::checkForPacket();
    vTaskDelay(pdMS_TO_TICKS(1));

  }
}

void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData,
                        bool extended)
{
  // Serial.print("Received packet with id 0x");
  // Serial.print(packetId, HEX);
  // Serial.println();

  // TODO handle messages from the rotary encoder

  TaskMessage::Message message = {
      .as = {.canMessage = {.id = packetId, .data = packetData}}};
  switch ((packetId & 0xFF00) >> 8)
  {
  case CYBERGEAR_CAN_ID_L:
    message.type = TaskMessage::Type::CAN_MESSAGE_MOTOR_L;
    xQueueSend(dataManagerQueue, &message, 0);
    break;
  case CYBERGEAR_CAN_ID_R:
    message.type = TaskMessage::Type::CAN_MESSAGE_MOTOR_R;
    xQueueSend(dataManagerQueue, &message, 0);
    break;
  default:
    Serial.print("Received packet from unknown device");
    Serial.print(" with id 0x");
    Serial.print(packetId, HEX);
    Serial.println();
    break;
  }
}
