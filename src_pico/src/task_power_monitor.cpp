#include "config.h"
#include "task_power_monitor.h"
#include "storage.h"

namespace TaskPowerMonitor
{
  CanardInstance canard;
  uint8_t memory_pool[1024];
  Storage::State &state = Storage::getState();
}

void TaskPowerMonitor::init()
{
  if (!CFG_ENABLE_POWER_MONITOR)
  {
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

void TaskPowerMonitor::receiveMessage(const Message &message)
{
  if (!CFG_ENABLE_POWER_MONITOR)
  {
    return;
  }
  switch (message.type)
  {
  case TaskPowerMonitor::MessageType::CAN_MESSAGE:
  {
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
  }
}

void TaskPowerMonitor::onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer)
{
  // switch on data type ID to pass to the right handler function
  if (transfer->transfer_type == CanardTransferTypeRequest)
  {
    // check if we want to handle a specific service request
    switch (transfer->data_type_id)
    {
    case UAVCAN_EQUIPMENT_POWER_BATTERYINFO_ID:
    {
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
                                            uint8_t source_node_id)
{
  return true;
}

void TaskPowerMonitor::handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer)
{
  uavcan_equipment_power_BatteryInfo msg{};
  if (uavcan_equipment_power_BatteryInfo_decode(transfer, &msg))
  {
    Serial.println("Failed to decode BatteryInfo message");
    return;
  }

  // There are 5 * 2 cells (parallel) each rated 3.6V so 3.6V * 5 = 18V
  // So we can expect 18V * 4Ah = 72Wh?
  // https://www.youtube.com/watch?v=WAKL66FG_0I

  static DataManager::Message taskMessage{};
  // Reset it
  taskMessage = DataManager::Message{};

  // TODO check every x ms consecutively in case we get a temporary voltage drop

  if (msg.voltage <= state.criticalVoltageThreshold)
  {
    Serial.println("[WARN] Battery voltage critical, shutting down");
    taskMessage.type = DataManager::Type::BATT_VOLTAGE_CRITICAL;
  }
  else if (msg.voltage <= state.lowVoltageThreshold)
  {
    Serial.println("[WARN] Battery voltage low, disabling motors");
    taskMessage.type = DataManager::Type::BATT_VOLTAGE_LOW;
  }
  else
  {
    taskMessage = {
        .type = DataManager::Type::BATT_OK,
        .as = {
            .battery = {
                .voltage = msg.voltage,
                .current = msg.current,
                .fuel = 4 * 4000, // 4 cells * 4000mAh
                .percent = 100    // TODO estimate percentage
            },
        },
    };
  }
  // This will be reentrant!
  DataManager::receiveMessage(taskMessage);
}