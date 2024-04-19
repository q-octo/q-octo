#pragma once

#include "task_data_manager.h"

namespace TaskDisplay
{
    typedef enum
    {
        STATE_UPDATE
    } MessageType;

    typedef struct
    {
        MessageType type;
        union
        {
            TaskMessage::State state;
        } as;
    } Message;
}

extern QueueHandle_t displayQueue;

void taskDisplay(void *pvParameters);
void taskDisplayPlaceholder(void *pvParameters);