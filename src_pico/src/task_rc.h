#pragma once

#include "task_data_manager.h"

namespace TaskRC
{
    typedef enum
    {
        BATTERY,
    } MessageType;
}

typedef struct
{
    TaskRC::MessageType type;
    union
    {
        TaskMessageBattery battery;
    } as;

} TaskRCMessage;

extern QueueHandle_t rcSendQueue;

void taskReceiveFromRC(void *pvParameters);
void taskSendToRC(void *pvParameters);
