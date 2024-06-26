#include <Arduino.h>
#include <can.h>
#include <canard.h>
#include <uavcan.equipment.power.BatteryInfo.h>
#include "uavcan.protocol.dynamic_node_id.Allocation.h"
#include "uavcan.protocol.NodeStatus.h"

extern "C"
{
#include "can2040.h"
}

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127

const uint8_t TEST_MOTOR_CAN_ID = CYBERGEAR_CAN_ID_L;

static CanardInstance canard;
static uint32_t memory_pool[1024];
uint8_t cached_node_id[16];
uint8_t cached_node_id_offset = 0;

static struct can2040 cbus;

void onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended);

void handleDynamicNodeIdAllocation(CanardInstance *ins, CanardRxTransfer *transfer);

void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer);

bool shouldAcceptTransfer(const CanardInstance *ins,
                          uint64_t *out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id);

void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer);

void transmitFrame(const CanardCANFrame *txf);

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg) {
  // Add message processing code here...
}

static void PIOx_IRQHandler(void) {
  can2040_pio_irq_handler(&cbus);
}

void canbus_setup(void) {
  uint32_t pio_num = 0;
  uint32_t sys_clock = 125000000, bitrate = 1'000'000;
  uint32_t gpio_rx = 11, gpio_tx = 10;

  // Setup canbus
  can2040_setup(&cbus, pio_num);
  can2040_callback_config(&cbus, can2040_cb);

  // Enable irqs
  irq_set_exclusive_handler(PIO0_IRQ_0_IRQn, PIOx_IRQHandler);
  NVIC_SetPriority(PIO0_IRQ_0_IRQn, 1);
  NVIC_EnableIRQ(PIO0_IRQ_0_IRQn);

  // Start canbus
  can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

void setup() {
  Serial.begin(115200);
  // Wait for serial connection to be established
  while (!Serial);
  delay(1000); // Wait for a second
  Serial.println("Live on core 0 ofc");
  // canbus_setup();
  CanCommunication::init(onReceiveCanPacket);
  Serial.println("Setup CAN");
  // Serial.println("Sending start motor message");
  // startMotor();

  // move(0, 0, 0, 0, 0);

  // Power monitor:
  // Initialize the Canard library
  Serial.println("Initializing Canard");
  delay(1000); // Wait for a second
  canardInit(&canard,
             memory_pool,
             sizeof(memory_pool),
             onTransferReceived,
             shouldAcceptTransfer,
             nullptr);

  Serial.println("Initialized Canard WHOOOO");

  canardSetLocalNodeID(&canard, 97);
  Serial.println("Initialized Canard");
}

void loop() {
  CanCommunication::checkForPacket();

  for (const CanardCANFrame *txf = NULL; (txf = canardPeekTxQueue(&canard)) != NULL;) {
    // Transmit the frame
    // Serial.printf("Transmitting frame\n");
    transmitFrame(txf);
    canardPopTxQueue(&canard);
  }
}

void transmitFrame(const CanardCANFrame *txf) {
  uint32_t id = txf->id;
  id &= 0x1FFFFFFF; // Keep only the lower 29 bits
  // Serial.printf("Transmitting frame with ID: %08X\n", id);
  // Serial.printf("Data: ");
  // for (int i = 0; i < txf->data_len; i++)
  // {
  //     Serial.printf("%02X ", txf->data[i]);
  // }
  // Serial.printf("\n");
  CanCommunication::sendCANPacket(id, txf->data, txf->data_len, true);
}

void onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended) {
  // Serial.print("Received packet with id 0x");
  // Serial.print(packetId, HEX);
  // Serial.println();

  // received a packet
  Serial.print("Received ");

  if (extended) {
    Serial.print("extended ");
  } else {
    Serial.print("standard ");
  }

  Serial.print("packet with id 0x");
  Serial.print(packetId, HEX);

  Serial.print(" and length ");
  Serial.println(packetLength);

  Serial.print("0x");
  for (size_t i = 0; i < packetLength; i++) {
    Serial.printf("%02X", packetData[i]);
  }
  Serial.println();

  // Check if the message is from the power monitor
  // Check the message type
  static CanardCANFrame rx_frame = {0};
  rx_frame.id = packetId | 0x80000000;
  rx_frame.data_len = packetLength;
  // "assume a single interface"
  rx_frame.iface_id = 0;
  memcpy(rx_frame.data, packetData, packetLength);

  static int lastDroneCanTransferId = -1;
  static int toggle = 1;
  // if bytes 2&3 of the id are 0444 then it's a battery info message
  // So we need to handle the toggle byte
  // int myParsedId = (rx_frame.id >> 8) & 0xFFFF;
  // Serial.print("Parsed ID: ");
  // Serial.println(myParsedId, HEX);
  // // if (myParsedId == 0x444)
  // // {
  //     Serial.println("intercepted battery info message");
  //     // last 5 bits of payload
  //     const uint8_t transferId = rx_frame.data[packetLength - 1] & 0x1F;
  //     if (transferId != lastDroneCanTransferId)
  //     {
  //         lastDroneCanTransferId = transferId;
  //         toggle = 1;
  //     }
  //     toggle = !toggle;
  //     Serial.print("Toggle: ");
  //     Serial.println(toggle);
  //     uint8_t currentToggle = (rx_frame.data[packetLength - 1] >> 5) & 0x01;
  //     if (currentToggle != toggle)
  //     {
  //         Serial.println("Toggle mismatch");
  //         toggle = !toggle;
  //         return;
  //     }
  // Set 6th bit of the last byte of the payload to the toggle value
  // rx_frame.data[packetLength - 1] = (rx_frame.data[packetLength - 1] & 0xDF) | (toggle << 5);

  // } else {
  // Serial.println()
  // }
  int16_t response = canardHandleRxFrame(&canard, &rx_frame, micros());
  Serial.print("Canard response: ??");
  Serial.println(response);
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
          Serial.printf("Received unknown droneCAN message with data type ID %d\n", transfer->data_type_id);
          break;
      }

      break;
    case CanardTransferTypeResponse:
    case CanardTransferTypeRequest:
      Serial.println("[WARN] Received a request/response transfer type");
      break;
  }
}

bool shouldAcceptTransfer(const CanardInstance *ins,
                          uint64_t *out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id) {
  if (transfer_type == CanardTransferTypeBroadcast) {
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
  }
  Serial.println("[WARN] Rejected transfer");
  return false;
}

void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer) {
  uavcan_equipment_power_BatteryInfo msg;
  if (uavcan_equipment_power_BatteryInfo_decode(transfer, &msg)) {
    Serial.println("Failed to decode BatteryInfo message");
    return;
  }

  Serial.print("Received battery info message: ");
  Serial.print("Voltage: ");
  Serial.print(msg.voltage);
  Serial.print("V, Current: ");
  Serial.print(msg.current);
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

  Serial.printf("Received DNA message: Node ID: %d, Unique ID: ", msg.node_id);
  for (int i = 0; i < msg.unique_id.len; i++) {
    Serial.printf("%02X ", msg.unique_id.data[i]);
  }
  Serial.printf("\n");

  const bool isFirstStageRequest = msg.first_part_of_unique_id;
  Serial.printf("Is first stage request: %d\n", isFirstStageRequest);
  // struct uavcan_protocol_dynamic_node_id_Allocation {
  // uint8_t node_id;
  // bool first_part_of_unique_id;
  // struct { uint8_t len; uint8_t data[16]; }unique_id;
  // };
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
  // Allocate memory for the response.unique_id.data field
  memcpy(response.unique_id.data, cached_node_id, cached_node_id_offset);
  response.unique_id.len = cached_node_id_offset;
  // Shouldn't be necessary
  response.first_part_of_unique_id = msg.first_part_of_unique_id;

  uint8_t buffer[UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_SIZE];
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