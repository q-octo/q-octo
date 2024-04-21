#include <Arduino.h>
#include <can.h>
#include <canard.h>
#include <uavcan.equipment.power.BatteryInfo.h>

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127

const uint8_t TEST_MOTOR_CAN_ID = CYBERGEAR_CAN_ID_L;

static CanardInstance canard;
static uint8_t memory_pool[1024];

void onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended);

void enableExtendedMode();
void startMotor();
void stopMotor();
void torqueControlMode();
void speedTorqueControlMode();
void positionSpeedTorqueTripleClosedLoopMode();
void setMotorPositionToZero();
void move(uint16_t position, uint16_t speed, uint16_t positionGain, uint16_t positionDamping, uint16_t torque);

void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer);
bool shouldAcceptTransfer(const CanardInstance *ins,
                          uint64_t *out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id);
void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer);

void setup()
{
    Serial.begin(115200);
    // Wait for serial connection to be established
    while (!Serial)
        ;
    delay(1000); // Wait for a second
    Serial.println("Live on core 0 ofc");
    CanCommunication::init(onReceiveCanPacket);
    // Serial.println("Sending start motor message");
    // startMotor();

    // move(0, 0, 0, 0, 0);

    // Power monitor:
    // Initialize the Canard library
    canardInit(&canard,
               memory_pool,
               sizeof(memory_pool),
               onTransferReceived,
               shouldAcceptTransfer,
               nullptr);
    // canardSetLocalNodeID(&canard, 97);
}

void loop()
{
    CanCommunication::checkForPacket();
}

void onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended)
{
    // Serial.print("Received packet with id 0x");
    // Serial.print(packetId, HEX);
    // Serial.println();

    // received a packet
    Serial.print("Received ");

    if (extended)
    {
        Serial.print("extended ");
    }
    else
    {
        Serial.print("standard ");
    }

    Serial.print("packet with id 0x");
    Serial.print(packetId, HEX);

    Serial.print(" and length ");
    Serial.println(packetLength);

    for (size_t i = 0; i < packetLength; i++)
    {
        Serial.print(packetData[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // Check if the message is from the power monitor
    // Check the message type
    CanardCANFrame rx_frame;
    rx_frame.id = packetId;
    rx_frame.data_len = packetLength;
    // "assume a single interface"
    rx_frame.iface_id = 0;
    memcpy(rx_frame.data, packetData, packetLength);
    int16_t response = canardHandleRxFrame(&canard, &rx_frame, micros());
    Serial.println("Canard response: ");
    Serial.println(response);

}



// Power monitor

void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer)
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

bool shouldAcceptTransfer(const CanardInstance *ins,
                          uint64_t *out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id)
{
    Serial.println("Accepting transfer");
    return true;
}

void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer)
{
    uavcan_equipment_power_BatteryInfo msg;
    if (uavcan_equipment_power_BatteryInfo_decode(transfer, &msg))
    {
        Serial.println("Failed to decode BatteryInfo message");
        return;
    }

    Serial.print("Received battery info message: ");
    Serial.print("Voltage: ");
    Serial.print(msg.voltage);
    Serial.print("V, Current: ");
    Serial.print(msg.current);
}