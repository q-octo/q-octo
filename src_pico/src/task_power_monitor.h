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
#pragma once

#include "task_data_manager.h"
#include "storage.h"
#include <canard.h>
#include <uavcan.equipment.power.BatteryInfo.h>
#include "crsf.h"

class TaskPowerMonitor {
public:
  typedef enum {
    CAN_MESSAGE,
    FLIGHT_CON_MESSAGE,
  } MessageType;

  typedef struct {
    MessageType type;
    union {
      DataManager::CanMessage canMessage;
      float voltageThreshold;
      int batteryCount;
      crsf_payload_battery_sensor_t crsf_battery;
    } as;

  } Message;

  static void init();

  static void receiveMessage(const Message &message);

private:

  static void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer);

  static bool shouldAcceptTransfer(const CanardInstance *ins, uint64_t *out_data_type_signature, uint16_t data_type_id,
                                   CanardTransferType transfer_type, uint8_t source_node_id);

  static void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer);
  static void handleBattery(const float voltage, const float current);

  static uint8_t voltageToBatteryPercent(float voltage);


  static inline CanardInstance canard;
  static inline uint32_t memory_pool[1024];
  static inline StorageState &state = Storage::getState();

};
