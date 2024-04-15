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

void onReceiveCanPacket(int packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended)
{
  // Serial.print("Received packet with id 0x");
  // Serial.print(packetId, HEX);
  // Serial.println();

  // TODO handle messages from the rotary encoder
  // TODO forward DroneCAN messages (CAN_MESSAGE_POWER_MONITOR)

  if (!extended)
  {
    Serial.println("[WARN] Received non-extended packet");
    return;
  }

  TaskMessage::Message message = {
      .as = {.canMessage = {.id = packetId, .data = packetData, .len = packetLength}}};

  const uint8_t asMotorId = (packetId & 0xFF00) >> 8;
  const uint8_t asMotorMasterCanID = (packetId & 0xFF);
  const bool matchesLeftMotor = asMotorId == CYBERGEAR_CAN_ID_L;
  const bool matchesRightMotor = asMotorId == CYBERGEAR_CAN_ID_R;
  
  // https://dronecan.github.io/Specification/4.1_CAN_bus_transport_layer/#can-frame-format
  const uint8_t asDroneCanSourceId = (packetId & 0x7F);

  // TODO log the packetId for motor messages.
  // What are the least significant 8 bits? (0-7)
  // 8-15 bits are the motor ID
  // What are 16-28?
  // They could all be zeros. The motor library ignores the id and just assumes
  // that the payload consists of position, speed, torque, and temperature.
  
  // If they are all zeros, it will be easy to distinguish between a motor
  // message and a power monitor message.
  if (matchesLeftMotor)
  {
    message.type = TaskMessage::Type::CAN_MESSAGE_MOTOR_L;
    xQueueSend(dataManagerQueue, &message, 0);
  }

  if (matchesRightMotor)
  {
    message.type = TaskMessage::Type::CAN_MESSAGE_MOTOR_R;
    xQueueSend(dataManagerQueue, &message, 0);
  }

  if (!matchesLeftMotor && !matchesRightMotor)
  {
    message.type = TaskMessage::Type::CAN_MESSAGE_POWER_MONITOR;
    xQueueSend(dataManagerQueue, &message, 0);
    Serial.print("Received packet from unknown device");
    Serial.print(" with id 0x");
    Serial.print(packetId, HEX);
    Serial.println();
    Serial.print("DroneCAN source id: 0x");
    Serial.print(asDroneCanSourceId, HEX);
    Serial.println();
  }
}
