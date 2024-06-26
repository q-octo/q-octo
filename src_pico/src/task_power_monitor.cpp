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
#include "task_power_monitor.h"
#include "storage.h"

// TODO strip out CAN logic once Flight Controller logic is confirmed to work

void TaskPowerMonitor::init() {
  if (!CFG_ENABLE_POWER_MONITOR) {
    return;
  }
  // Initialize the Canard library
  canardInit(&canard,
             memory_pool,
             sizeof(memory_pool),
             onTransferReceived,
             shouldAcceptTransfer,
             nullptr);
  Serial.println("Power monitor initialised");
}

void TaskPowerMonitor::receiveMessage(const Message &message) {
  if (!CFG_ENABLE_POWER_MONITOR) {
    return;
  }
  switch (message.type) {
    case TaskPowerMonitor::MessageType::CAN_MESSAGE: {
      // Check if the message is from the power monitor
      // Check the message type
      CanardCANFrame rx_frame;
      auto msg = message.as.canMessage;
      rx_frame.id = msg.id;
      rx_frame.data_len = msg.len;
      // "assume a single interface"
      rx_frame.iface_id = 0;
      memcpy(rx_frame.data, msg.data, msg.len);
      canardHandleRxFrame(&canard, &rx_frame, micros());
      break;
    }
    case MessageType::FLIGHT_CON_MESSAGE: {
      auto battery = message.as.crsf_battery;
      handleBattery(battery.voltage, battery.current);
    }
  }
}

void TaskPowerMonitor::onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer) {
  // switch on data type ID to pass to the right handler function
  if (transfer->transfer_type == CanardTransferTypeRequest) {
    // check if we want to handle a specific service request
    switch (transfer->data_type_id) {
      case UAVCAN_EQUIPMENT_POWER_BATTERYINFO_ID: {
        handlePowerBatteryInfo(ins, transfer);
        break;
      }
      default:
        // TODO We should handle status messages from the power monitor
        Serial.print("[WARN] Received unknown droneCAN message");
        Serial.print(" with data type ID ");
        Serial.println(transfer->data_type_id);
        break;
    }
  }
}

bool TaskPowerMonitor::shouldAcceptTransfer(const CanardInstance *ins,
                                            uint64_t *out_data_type_signature,
                                            uint16_t data_type_id,
                                            CanardTransferType transfer_type,
                                            uint8_t source_node_id) {
  return true;
}



/// @brief Converts battery voltage to a percentage.
uint8_t TaskPowerMonitor::voltageToBatteryPercent(float voltage) {
  // Essentially, at full charge, we'll be over 18V but the drop will be sharp.
  // So we could reserve the top 5% for the initial drop.

  const float stableUpperBound = 18.0f;
  
  if (voltage > stableUpperBound) {
    // We will actually start at ~21V when fully charged. Reserve the top 5%
    // for this case.
    
    const float range = 21.0f - stableUpperBound;
    const float remainingVoltage = voltage - stableUpperBound;
    const float percent = ((remainingVoltage / range) * 5.0f) + 95.0f;
  }


  // Min voltage is 17V, max is 18v.
  auto &state = Storage::getState();
  
  const float range = stableUpperBound - state.criticalVoltageThreshold;
  const float remainingVoltage = voltage - state.criticalVoltageThreshold;
  if (remainingVoltage <= 0) {
    return 0;
  }

  const float percent = (remainingVoltage / range) * 95.0f;
  if (percent > 95) {
    return 95;
  }
  return static_cast<uint8_t>(percent);
}


void TaskPowerMonitor::handleBattery(const float voltage, const float current) {
  static DataManager::Message taskMessage{};
  // Reset it
  taskMessage = DataManager::Message{};

  // There are 5 * 2 cells (parallel) each rated 3.6V so 3.6V * 5 = 18V
  // So we can expect 18V * 4Ah = 72Wh?
  // https://www.youtube.com/watch?v=WAKL66FG_0I
  
  if (voltage <= state.criticalVoltageThreshold) {
    Serial.println("[WARN] Battery voltage critical, shutting down");
    taskMessage.type = DataManager::Type::BATT_VOLTAGE_CRITICAL;
  } else if (voltage <= state.lowVoltageThreshold) {
    Serial.println("[WARN] Battery voltage low, disabling motors");
    taskMessage.type = DataManager::Type::BATT_VOLTAGE_LOW;
  } else {
    const uint8_t batteryPercent = voltageToBatteryPercent(voltage);
    // cells * 4000mAh
    const uint32_t batteryFuel = state.batteryCount * 4000;
    taskMessage = {
            .type = DataManager::Type::BATT_OK,
            .as = {
                    .battery = {
                            .voltage = voltage,
                            .current = current,
                            .fuel = batteryFuel,
                            .percent = batteryPercent,
                    },
            },
    };
  }
  // This will be reentrant (in the CAN case)!
  DataManager::receiveMessage(taskMessage);
}

void TaskPowerMonitor::handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer) {
  uavcan_equipment_power_BatteryInfo msg{};
  if (uavcan_equipment_power_BatteryInfo_decode(transfer, &msg)) {
    Serial.println("Failed to decode BatteryInfo message");
    return;
  }

  // TODO check every x ms consecutively in case we get a temporary voltage drop
  handleBattery(msg.voltage, msg.current);
}