#pragma once

#include "task_data_manager.h"

namespace TaskRC
{
    typedef enum
    {
        BATTERY,
    } MessageType;


typedef struct
{
    MessageType type;
    union
    {
        TaskMessage::Battery battery;
    } as;

} Message;

}

extern QueueHandle_t rcSendQueue;

void taskReceiveFromRC(void *pvParameters);
void taskSendToRC(void *pvParameters);
