#pragma once

#include "task_data_manager.h"

namespace TaskPowerMonitor
{
    typedef enum
    {
        CAN_MESSAGE,
        SET_LOW_VOLTAGE_THRESHOLD,
        SET_CRITICAL_VOLTAGE_THRESHOLD,
        SET_BATTERY_COUNT,
    } MessageType;

    typedef struct
    {
        MessageType type;
        union
        {
            TaskMessage::CanMessage canMessage;
            float voltageThreshold;
            int batteryCount;
        } as;

    } Message;
}

extern QueueHandle_t powerMonitorQueue;

void taskPowerMonitor(void *pvParameters);