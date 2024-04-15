#include "config.h"
#include "task_power_monitor.h"

QueueHandle_t powerMonitorQueue;

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

    for (;;)
    {
        if (xQueueReceive(powerMonitorQueue, &message, portMAX_DELAY))
        {
            switch (message.type)
            {
            case TaskPowerMonitor::MessageType::CAN_MESSAGE:
                break;
            }
        }
    }
}