#ifndef TASK_DISPLAY_H
#define TASK_DISPLAY_H

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

}
typedef struct
{
    TaskDisplay::MessageType type;
    union
    {
        TaskMessageBattery battery;
        TaskMessageDiagnostics diagnostics;
        TaskMessageRC rc;
        TaskMessageMotors motors;
    };
} TaskDisplayMessage;

extern QueueHandle_t displayQueue;

void taskDisplay(void *pvParameters);

#endif // TASK_DISPLAY_H