#pragma once

#include "task_data_manager.h"
#include "storage.h"
#include <canard.h>
#include <uavcan.equipment.power.BatteryInfo.h>

class TaskPowerMonitor {
public:
  typedef enum {
    CAN_MESSAGE,
  } MessageType;

  typedef struct {
    MessageType type;
    union {
      DataManager::CanMessage canMessage;
      float voltageThreshold;
      int batteryCount;
    } as;

  } Message;

  static void init();

  static void receiveMessage(const Message &message);

private:

  static void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer);

  static bool shouldAcceptTransfer(const CanardInstance *ins, uint64_t *out_data_type_signature, uint16_t data_type_id,
                                   CanardTransferType transfer_type, uint8_t source_node_id);

  static void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer);

  static uint8_t voltageToBatteryPercent(float voltage);


  static inline CanardInstance canard;
  static inline uint8_t memory_pool[1024];
  static inline Storage::State &state = Storage::getState();

};
