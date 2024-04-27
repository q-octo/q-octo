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

void transmitFrame(const CanardCANFrame *txf) {
  printf("Transmitting frame with ID: %08X\n", txf->id);
  printf("Data: ");
  for (int i = 0; i < txf->data_len; i++) {
    printf("%02X ", txf->data[i]);
  }
  printf("\n");
}

int main() {
  canardInit(&canard,
             memory_pool,
             sizeof(memory_pool),
             onTransferReceived,
             shouldAcceptTransfer,
             NULL);

  // Assign ourselves a node ID
  canardSetLocalNodeID(&canard, 97);


  struct uavcan_equipment_power_BatteryInfo msg;
  memset(&msg, 0, sizeof(msg));
  uint8_t buffer[UAVCAN_EQUIPMENT_POWER_BATTERYINFO_MAX_SIZE];
  uint32_t len = uavcan_equipment_power_BatteryInfo_encode(&msg, buffer);
  printf("Encoded message length: %u\n", len);


  static uint8_t transferId;

//  canardBroadcast(&canard,
//                  UAVCAN_EQUIPMENT_POWER_BATTERYINFO_SIGNATURE,
//                  UAVCAN_EQUIPMENT_POWER_BATTERYINFO_ID,
//                  &transferId,
//                  24,
//                  buffer,
//                  len
//  );
//
//  for (;;) {
//    for (const CanardCANFrame *txf = NULL; (txf = canardPeekTxQueue(&canard)) != NULL;) {
//      // Transmit the frame
//      transmitFrame(txf);
//      canardPopTxQueue(&canard);
//    }
//  }

// Demo payload received from the PM
  const uint8_t data[] = {
          0x00, 0x00, 0x00, 0x00, 0xEC,
          0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x01,
          0x42, 0x61, 0x74, 0x74, 0x4D, 0x6F, 0x6E,
          0x20, 0x2D, 0x31
  };

  struct uavcan_equipment_power_BatteryInfo pkt;
  memset(&pkt, 0, sizeof(pkt));
  CanardRxTransfer transfer = {
          .timestamp_usec = 0,
          .priority = 24,
          .transfer_id = 0,
          .source_node_id = 120,
          .transfer_type = CanardTransferTypeBroadcast,
          .data_type_id = UAVCAN_EQUIPMENT_POWER_BATTERYINFO_ID,
          .payload_len = sizeof(data),
          .payload_head = data,
          .payload_tail = data + sizeof(data),
  };
  uavcan_equipment_power_BatteryInfo_decode(&transfer, &pkt);



  return 0;

//  5 + 7 + 7 + 3 = 22
// 54 - 32 (length of name array) = 22

  const uint32_t id = 0x18044478;
//  0x5E 7C 00 00 43 4E EC 80
//  0x0000001FC0000100
//  0x426174744D6F6E20
//  0x202D3140
  const uint8_t data1[] = {0x5E, 0x7C,
                           0x00, 0x00, 0x43, 0x4E, 0xEC,
                           0x80};
  const uint8_t data2[] = {0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x01,
                           0x00};
//                           0x20};
  const uint8_t data3[] = {0x42, 0x61, 0x74, 0x74, 0x4D, 0x6F, 0x6E,
                           0x20};
//                           0x00};
  const uint8_t data4[] = {0x20, 0x2D, 0x31,
                           0x40};
//  0x60};

  receiveCanPacket(8, id, data1);
  receiveCanPacket(8, id, data2);
  receiveCanPacket(8, id, data3);
  receiveCanPacket(4, id, data4);

  for (;;) {
    // // Send message with this 29 bit id: 0x18F47500
    // // But set the top 3 bits to 100 to indicate that it is an extended frame
    // // The bottom 29 bits are the actual message id
    // uint32_t id = 0x18F47500;
    // // DATA: 01 43 00 29 00 08 50 C0
    // uint8_t data[8] = {0x01, 0x43, 0x00, 0x29, 0x00, 0x08, 0x50, 0xC0};
    // receiveCanPacket(8, id, data);

    for (const CanardCANFrame *txf = NULL; (txf = canardPeekTxQueue(&canard)) != NULL;) {
      // Transmit the frame
      printf("Transmitting frame\n");
      transmitFrame(txf);
      canardPopTxQueue(&canard);
    }
    sleep(1);
  }
}

void receiveCanPacket(uint8_t packetLength, uint32_t packetId, const uint8_t *packetData) {
  // Check if the message is from the power monitor
  // Check the message type
  static CanardCANFrame rx_frame;
  rx_frame.id = packetId | 0x80000000;
  rx_frame.data_len = packetLength;
  // "assume a single interface"
  rx_frame.iface_id = 0;
  memcpy(rx_frame.data, packetData, packetLength);
  static uint64_t timestamp = 1;
  int16_t response = canardHandleRxFrame(&canard, &rx_frame, timestamp++);
  printf("Canard response: %d\n", response);
}

// Power monitor
void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer) {

  // switch on data type ID to pass to the right handler function
  switch (transfer->transfer_type) {
    case CanardTransferTypeBroadcast:
      switch (transfer->data_type_id) {
        // check if we want to handle a specific service request
        case UAVCAN_EQUIPMENT_POWER_BATTERYINFO_ID:
          handlePowerBatteryInfo(ins, transfer);
          break;
        case UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_ID:
          // Handle DNA message
          handleDynamicNodeIdAllocation(ins, transfer);
          break;
        case UAVCAN_PROTOCOL_NODESTATUS_ID:
          break;
        default:
          // TODO We should handle status messages from the power monitor
          printf("Received unknown droneCAN message with data type ID %d\n", transfer->data_type_id);
          break;
      }

      break;
    case CanardTransferTypeResponse:
    case CanardTransferTypeRequest:
      printf("[WARN] Received a request/response transfer type\n");
      break;

  }
}

bool shouldAcceptTransfer(const CanardInstance *ins,
                          uint64_t *out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id) {
  switch (data_type_id) {
    case UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_ID:
      *out_data_type_signature = UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_SIGNATURE;
      return true;
    case UAVCAN_EQUIPMENT_POWER_BATTERYINFO_ID:
      *out_data_type_signature = UAVCAN_EQUIPMENT_POWER_BATTERYINFO_SIGNATURE;
      return true;
    case UAVCAN_PROTOCOL_NODESTATUS_ID:
      *out_data_type_signature = UAVCAN_PROTOCOL_NODESTATUS_SIGNATURE;
      return true;
  }

  printf("[WARN] Rejected transfer with data type ID %d\n", data_type_id);

  return false;
}

void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer) {
  struct uavcan_equipment_power_BatteryInfo msg;
  if (uavcan_equipment_power_BatteryInfo_decode(transfer, &msg)) {
    // Serial.println("Failed to decode BatteryInfo message");
    return;
  }

  // Serial.print("Received battery info message: ");
  // Serial.print("Voltage: ");
  // Serial.print(msg.voltage);
  // Serial.print("V, Current: ");
  // Serial.print(msg.current);
}

void handleDynamicNodeIdAllocation(CanardInstance *ins, CanardRxTransfer *transfer) {
  static uint8_t node_id_allocation_transfer_id = 0;

  struct uavcan_protocol_dynamic_node_id_Allocation msg;
  if (uavcan_protocol_dynamic_node_id_Allocation_decode(transfer, &msg)) {
    // Serial.println("Failed to decode DNA message");
    return;
  }

  // Node id is 0

  //  msg.node_id  is 0
  //  msg.first_part_of_unique_id is true
  // msg.unique_id.len is 6

  printf("Received DNA message: Node ID: %d, Unique ID: ", msg.node_id);
  for (int i = 0; i < msg.unique_id.len; i++) {
    printf("%02X ", msg.unique_id.data[i]);
  }
  printf("\n");

  const bool isFirstStageRequest = msg.first_part_of_unique_id;
  struct uavcan_protocol_dynamic_node_id_Allocation response;
  memset(&response, 0, sizeof(response));
  // 125 or lower is allowed
  response.node_id = 120;

  assert(msg.unique_id.len <= UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_LENGTH_OF_UNIQUE_ID_IN_REQUEST);
  if (isFirstStageRequest) {
    cached_node_id_offset = 0;
  }

  // The allocatee will send their 16 byte node ID in 6 byte chunks
  // We will store these chunks in the cached_node_id buffer
  // And at each stage, we will send back all the accumulated chunks
  memcpy(cached_node_id + cached_node_id_offset, msg.unique_id.data, msg.unique_id.len);
  cached_node_id_offset += msg.unique_id.len;
  memcpy(response.unique_id.data, cached_node_id, cached_node_id_offset);
  response.unique_id.len = cached_node_id_offset;

  uint8_t buffer[1000];
  uint32_t len = uavcan_protocol_dynamic_node_id_Allocation_encode(&response, buffer);

  canardBroadcast(&canard,
                  UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_SIGNATURE,
                  UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_ID,
                  &node_id_allocation_transfer_id, // transfer ID
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