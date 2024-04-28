#pragma once

#include "task_data_manager.h"
#include <canard.h>
#include <uavcan.equipment.power.BatteryInfo.h>

namespace TaskPowerMonitor
{
  // public:
  typedef enum
  {
    CAN_MESSAGE,
  } MessageType;

  typedef struct
  {
    MessageType type;
    union
    {
      DataManager::CanMessage canMessage;
      float voltageThreshold;
      int batteryCount;
    } as;

  } Message;

  void init();

  void receiveMessage(const Message &message);

  // private:
  void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer);

  bool shouldAcceptTransfer(const CanardInstance *ins, uint64_t *out_data_type_signature, uint16_t data_type_id,
                            CanardTransferType transfer_type, uint8_t source_node_id);

  void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer);
  uint8_t voltageToBatteryPercent(float voltage);
  
}
