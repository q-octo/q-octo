#pragma once

#include "task_data_manager.h"

namespace TaskPowerMonitor
{
    typedef enum
    {
        CAN_MESSAGE,
    } MessageType;

    typedef struct
    {
        MessageType type;
        union
        {
            TaskMessage::CanMessage canMessage;
        } as;

    } Message;
}

extern QueueHandle_t powerMonitorQueue;

void taskPowerMonitor(void *pvParameters);