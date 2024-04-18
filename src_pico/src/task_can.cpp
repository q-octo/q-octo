#include "task_can.h"
#include "can.h"
#include "task_motors.h"
#include "task_data_manager.h"

#include "FreeRTOS.h"
#include "queue.h"

bool isPowerMonitorPacket(uint32_t packetId);

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
      .as = {.canMessage =
                 {.id = packetId, .data = packetData, .len = packetLength}}};

  const uint8_t motorCommType = packetId >> 24;
  // TODO verify that MASTER_CAN_ID is correct before enforcing this check.
  const uint8_t motorHostId = packetId & 0xFF;
  bool isMotorPacket = motorCommType == 0x02 || motorCommType == 0x15;
  const bool isPowerMonitor = isPowerMonitorPacket(packetId);

  if (isPowerMonitor)
  {
    message.type = TaskMessage::Type::CAN_MESSAGE_POWER_MONITOR;
    xQueueSend(dataManagerQueue, &message, 0);
    return;
  }

  if (isMotorPacket)
  {
    switch (motorCommType)
    {
    case 0x02:
    {
      const uint8_t motorId = (packetId & 0xFF00) >> 8;

      if (motorId == CYBERGEAR_CAN_ID_L)
      {
        message.type = TaskMessage::Type::CAN_MESSAGE_MOTOR_L;
        xQueueSend(dataManagerQueue, &message, 0);
      }
      if (motorId == CYBERGEAR_CAN_ID_R)
      {
        message.type = TaskMessage::Type::CAN_MESSAGE_MOTOR_R;
        xQueueSend(dataManagerQueue, &message, 0);
      }
      break;
    }
    case 0x15:
    {
      // Just send to both motors for now
      message.type = TaskMessage::Type::CAN_MESSAGE_MOTOR_L;
      xQueueSend(dataManagerQueue, &message, 0);
      message.type = TaskMessage::Type::CAN_MESSAGE_MOTOR_R;
      xQueueSend(dataManagerQueue, &message, 0);
      break;
    }
    default:
    {
      isMotorPacket = false;
      break;
    }
    }
  }

  if (!isMotorPacket && !isPowerMonitor)
  {
    // https://dronecan.github.io/Specification/4.1_CAN_bus_transport_layer/#can-frame-format
    const uint8_t asDroneCanSourceId = (packetId & 0x7F);
    const uint16_t asDroneCanMessageTypeId = (packetId & 0xFFFF00) >> 8;
    Serial.print("Received packet from unknown device");
    Serial.print(" with id 0x");
    Serial.print(packetId, HEX);
    Serial.println();
    Serial.print("DroneCAN source id: 0x"); // hypothetical
    Serial.print(asDroneCanSourceId, HEX);
    Serial.println();
    Serial.print("DroneCAN message type id: 0x"); // hypothetical
    Serial.print(asDroneCanMessageTypeId, HEX);
    Serial.println();
  }
}

bool isPowerMonitorPacket(uint32_t packetId)
{
  // Assuming that we only expect the message frame type.
  const uint16_t messageTypeId = (packetId & 0xFFFF00) >> 8;
  const uint16_t batteryInfoDataTypeId = 1092;
  return messageTypeId == batteryInfoDataTypeId;
}