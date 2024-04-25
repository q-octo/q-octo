#include "config.h"
#include "task_power_monitor.h"
#include <canard.h>
#include <uavcan.equipment.power.BatteryInfo.h>

static CanardInstance canard;
static uint8_t memory_pool[1024];

float motorShutdownVoltage = 17.5f;
float systemShutdownVoltage = 17.0f;
int batteryCount = 4;

QueueHandle_t powerMonitorQueue;

void onTransferReceived(CanardInstance *ins, CanardRxTransfer *transfer);
bool shouldAcceptTransfer(const CanardInstance *ins,
                          uint64_t *out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id);
void handlePowerBatteryInfo(CanardInstance *ins, CanardRxTransfer *transfer);

void taskPowerMonitor(void *pvParameters)
{
    Serial.println("taskPowerMonitor started");
    powerMonitorQueue = xQueueCreate(10, sizeof(TaskPowerMonitor::Message));
    if (powerMonitorQueue == nullptr)
    {
        Serial.println("Failed to create powerMonitorQueue");
        vTaskDelete(nullptr);
    }

    static TaskPowerMonitor::Message message;
#if !CFG_ENABLE_POWER_MONITOR
    Serial.println("Power monitor disabled, blocking indefinitely");
    for (;;)
    {
        xQueueReceive(powerMonitorQueue, &message, portMAX_DELAY);
    }
#endif

    // Initialize the Canard library
    canardInit(&canard,
               memory_pool,
               sizeof(memory_pool),
               onTransferReceived,
               shouldAcceptTransfer,
               nullptr);

    for (;;)
    {
        if (xQueueReceive(powerMonitorQueue, &message, portMAX_DELAY))
        {
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
            case TaskPowerMonitor::MessageType::SET_LOW_VOLTAGE_THRESHOLD:
            {
                motorShutdownVoltage = message.as.voltageThreshold;
                break;
            }
            case TaskPowerMonitor::MessageType::SET_CRITICAL_VOLTAGE_THRESHOLD:
            {
                systemShutdownVoltage = message.as.voltageThreshold;
                break;
            }
            case TaskPowerMonitor::SET_BATTERY_COUNT:
            {
                batteryCount = message.as.batteryCount;
                break;
            }
            }
        }
    }
}

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

    // There are 5 * 2 cells (parallel) each rated 3.6V so 3.6V * 5 = 18V
    // So we can expect 18V * 4Ah = 72Wh?
    // https://www.youtube.com/watch?v=WAKL66FG_0I

    static TaskMessage::Message taskMessage;
    memset(&taskMessage, 0, sizeof(taskMessage));

    if (msg.voltage <= systemShutdownVoltage)
    {
        Serial.println("[WARN] Battery voltage critical, shutting down");
        taskMessage.type = TaskMessage::Type::BATT_VOLTAGE_CRITICAL;
    }
    else if (msg.voltage <= motorShutdownVoltage)
    {
        Serial.println("[WARN] Battery voltage low, disabling motors");
        taskMessage.type = TaskMessage::Type::BATT_VOLTAGE_LOW;
    }
    else
    {
        taskMessage = {
            .type = TaskMessage::Type::BATT_OK,
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
    xQueueSend(dataManagerQueue, &taskMessage, 0);
}