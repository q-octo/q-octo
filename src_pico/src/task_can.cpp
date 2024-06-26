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
#include "config.h"
#include "task_can.h"
#include "can.h"
#include "task_motors.h"
#include "task_data_manager.h"

void TaskCAN::init() {
  if (!CFG_ENABLE_CAN) {
    return;
  }
  // IMPORTANT that this occurs outside a FreeRTOS task (maybe so that we
  // don't send a CAN message before CAN is initialized?)
  CanCommunication::init(onReceiveCanPacket);
  Serial.println("TaskCAN::init complete");
}

void TaskCAN::loop() {
  if (!CFG_ENABLE_CAN) {
    return;
  }
  CanCommunication::checkForPacket();
}

void TaskCAN::onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                                 bool extended) {
  // Serial.print("Received packet with id 0x");
  // Serial.print(packetId, HEX);
  // Serial.println();

  // TODO handle messages from the rotary encoder
  // TODO forward DroneCAN messages (CAN_MESSAGE_POWER_MONITOR)

  if (!extended) {
    Serial.println("[WARN] Received non-extended packet");
    return;
  }

  DataManager::Message message = {
          .as = {.canMessage =
                  {.id = packetId, .data = packetData, .len = packetLength}}};

  const uint8_t motorCommType = packetId >> 24;
  // TODO verify that MASTER_CAN_ID is correct before enforcing this check.
  const uint8_t motorHostId = packetId & 0xFF;
  bool isMotorPacket = motorCommType == 0x02 || motorCommType == 0x15 || motorCommType == 0x11;
  const bool isPowerMonitor = isPowerMonitorPacket(packetId);

  // if (isPowerMonitor) {
  //   message.type = DataManager::Type::CAN_MESSAGE_POWER_MONITOR;
  //   DataManager::receiveMessage(message);
  //   return;
  // }

  if (isMotorPacket) {
    switch (motorCommType) {
      case 0x11:
      case 0x15:
      case 0x02: // Motor feedback data
      {
        const uint8_t motorId = (packetId & 0xFF00) >> 8;

        if (motorId == CYBERGEAR_CAN_ID_L) {
          message.type = DataManager::Type::CAN_MESSAGE_MOTOR_L;
          DataManager::receiveMessage(message);
        }
        if (motorId == CYBERGEAR_CAN_ID_R) {
          message.type = DataManager::Type::CAN_MESSAGE_MOTOR_R;
          DataManager::receiveMessage(message);
        }
        break;
      }
      default: {
        isMotorPacket = false;
        break;
      }
    }
  }

  if (!isMotorPacket && !isPowerMonitor) {
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

bool TaskCAN::isPowerMonitorPacket(uint32_t packetId) {
  // Assuming that we only expect the message frame type.
  const uint16_t messageTypeId = (packetId & 0xFFFF00) >> 8;
  const uint16_t batteryInfoDataTypeId = 1092;
  return messageTypeId == batteryInfoDataTypeId;
}