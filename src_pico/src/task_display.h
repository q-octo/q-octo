#pragma once

#include "task_data_manager.h"

namespace TaskDisplay
{
    typedef enum
    {
        MOTORS,
        BATTERY,
        RC,
        DIAGNOSTICS,
    } MessageType;

    typedef struct
    {
        MessageType type;
        union
        {
            TaskMessage::Battery battery;
            TaskMessage::Diagnostics diagnostics;
            TaskMessage::RC rc;
            TaskMessage::Motors motors;
        } as;
    } Message;
}

extern QueueHandle_t displayQueue;

void taskDisplay(void *pvParameters);