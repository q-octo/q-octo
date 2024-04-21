#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "canard.h"
#include "uavcan.equipment.power.BatteryInfo.h"
#include "uavcan.protocol.dynamic_node_id.Allocation.h"
#include "uavcan.protocol.NodeStatus.h"
#include <unistd.h>

static CanardInstance canard;
static uint8_t memory_pool[1024];
uint8_t cached_node_id[16];
uint8_t cached_node_id_offset = 0;

void receiveCanPacket(uint8_t packetLength, uint32_t packetId, const uint8_t *packetData);
void handleDynamicNodeIdAllocation(CanardInstance *ins, CanardRxTransfer *transfer);

void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer);

bool shouldAcceptTransfer(const CanardInstance *ins,
                          uint64_t *out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id);

void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer);

int main()
{
  canardInit(&canard,
             memory_pool,
             sizeof(memory_pool),
             onTransferReceived,
             shouldAcceptTransfer,
             NULL);

  // Assign ourselves a node ID
  canardSetLocalNodeID(&canard, 97);

  for (;;)
  {
    // Send message with this 29 bit id: 0x18F47500
    // But set the top 3 bits to 100 to indicate that it is an extended frame
    // The bottom 29 bits are the actual message id
    uint32_t id = 0x18F47500;
    // DATA: 01 43 00 29 00 08 50 C0
    uint8_t data[8] = {0x01, 0x43, 0x00, 0x29, 0x00, 0x08, 0x50, 0xC0};
    receiveCanPacket(8, id, data);
    sleep(1);
  }
}

void receiveCanPacket(uint8_t packetLength, uint32_t packetId, const uint8_t *packetData)
{
  // Check if the message is from the power monitor
  // Check the message type
  CanardCANFrame rx_frame;
  rx_frame.id = packetId | 0x80000000;
  rx_frame.data_len = packetLength;
  // "assume a single interface"
  rx_frame.iface_id = 0;
  memcpy(rx_frame.data, packetData, packetLength);
  int16_t response = canardHandleRxFrame(&canard, &rx_frame, 0);
  printf("Canard response: %d\n", response);
}

// Power monitor
void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer)
{
  // switch on data type ID to pass to the right handler function
  switch (transfer->transfer_type)
  {
  case CanardTransferTypeRequest:
    switch (transfer->data_type_id)
    {

    // check if we want to handle a specific service request
    case UAVCAN_EQUIPMENT_POWER_BATTERYINFO_ID:
    {
      handlePowerBatteryInfo(ins, transfer);
      break;
    }
    default:
      // TODO We should handle status messages from the power monitor
      printf("Received unknown droneCAN message with data type ID %d\n", transfer->data_type_id);
      break;
    }
    break;
  case CanardTransferTypeBroadcast:
    // Now parse DNA message
    if (transfer->data_type_id == UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_ID)
    {
      // Handle DNA message
      handleDynamicNodeIdAllocation(ins, transfer);
    }
    break;
  case CanardTransferTypeResponse:
    break;
  }
}

bool shouldAcceptTransfer(const CanardInstance *ins,
                          uint64_t *out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id)
{
  if (transfer_type == CanardTransferTypeBroadcast && data_type_id == UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_ID)
  {
    // Dyamic Node Allocation request.
    *out_data_type_signature = UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_SIGNATURE;
    return true;
  }

  if (transfer_type == CanardTransferTypeRequest && data_type_id == UAVCAN_EQUIPMENT_POWER_BATTERYINFO_ID)
  {
    // Battery info request
    return true;
  }

  // TODO print warning
  return false;
}

void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer)
{
  struct uavcan_equipment_power_BatteryInfo msg;
  if (uavcan_equipment_power_BatteryInfo_decode(transfer, &msg))
  {
    // Serial.println("Failed to decode BatteryInfo message");
    return;
  }

  // Serial.print("Received battery info message: ");
  // Serial.print("Voltage: ");
  // Serial.print(msg.voltage);
  // Serial.print("V, Current: ");
  // Serial.print(msg.current);
}

void handleDynamicNodeIdAllocation(CanardInstance *ins, CanardRxTransfer *transfer)
{
  struct uavcan_protocol_dynamic_node_id_Allocation msg;
  if (uavcan_protocol_dynamic_node_id_Allocation_decode(transfer, &msg))
  {
    // Serial.println("Failed to decode DNA message");
    return;
  }

  // Node id is 0

  //  msg.node_id  is 0
  //  msg.first_part_of_unique_id is true
  // msg.unique_id.len is 6

  printf("Received DNA message: Node ID: %d, Unique ID: ", msg.node_id);
  for (int i = 0; i < msg.unique_id.len; i++)
  {
    printf("%02X ", msg.unique_id.data[i]);
  }
  printf("\n");

  const bool isFirstStageRequest = msg.first_part_of_unique_id;
  struct uavcan_protocol_dynamic_node_id_Allocation response;
  memset(&response, 0, sizeof(response));
  // 125 or lower is allowed
  response.node_id = 120;

  assert(msg.unique_id.len <= UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_LENGTH_OF_UNIQUE_ID_IN_REQUEST);
  if (isFirstStageRequest)
  {
    cached_node_id_offset = 0;
  }

  // The allocatee will send their 16 byte node ID in 6 byte chunks
  // We will store these chunks in the cached_node_id buffer
  // And at each stage, we will send back all the accumulated chunks
  memcpy(cached_node_id + cached_node_id_offset, msg.unique_id.data, msg.unique_id.len);
  cached_node_id_offset += msg.unique_id.len;
  memcpy(response.unique_id.data, cached_node_id, cached_node_id_offset);
  response.unique_id.len = cached_node_id_offset;

  uint8_t buffer[UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_SIZE];
  uint32_t len = uavcan_protocol_dynamic_node_id_Allocation_encode(&response, buffer);

  canardBroadcast(&canard,
                  UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_SIGNATURE,
                  UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_ID,
                  0, // transfer ID
                  CANARD_TRANSFER_PRIORITY_LOW,
                  buffer,
                  len);

  // Serial.print("Received DNA message: ");
  // Serial.print("Node ID: ");
  // Serial.print(msg.node_id);
  // Serial.print(", Unique ID: ");
  // for (int i = 0; i < msg.unique_id.len; i++) {
  //   Serial.print(msg.unique_id.data[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();
}