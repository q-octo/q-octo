#include "config.h"
#include "task_watchdog.h"

#define WATCHDOG_TIMEOUT 5000
#define LOG_MEMORY_FREQUENCY 10000
#define HEALTH_CHECK_FREQUENCY 100

uint32_t lastMemoryLog = 0;
uint32_t lastHealthCheckMs = 0;

QueueHandle_t watchdogQueue;
long tasks[Task::numValues] = {0};

void taskWatchdog(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings
    Serial.println("taskWatchdog started");
    watchdogQueue = xQueueCreate(10, sizeof(Task));
    if (watchdogQueue == nullptr)
    {
        Serial.println("Failed to create watchdogQueue");
        vTaskDelete(nullptr);
    }
#if !CFG_ENABLE_WATCHDOG
    Serial.println("Watchdog disabled, blocking indefinitely");
    for (;;)
    {
        Task message;
        xQueueReceive(watchdogQueue, &message, portMAX_DELAY);
    }
#endif

    // if (watchdog_caused_reboot())
    // {
    //     Serial.println("RECOVERED FROM WATCHDOG REBOOT");
    // }
    // rp2040.wdt_begin(WATCHDOG_TIMEOUT);
    for (;;)
    {
        Task task;
        BaseType_t xStatus = xQueueReceive(watchdogQueue, &task, pdMS_TO_TICKS(1));
        const bool messageReceived = xStatus == pdPASS;
        if (messageReceived)
        {
            tasks[task] += 1;
        }
        const uint32_t currentMillis = millis();
        // if (currentMillis - lastHealthCheckMs > HEALTH_CHECK_FREQUENCY)
        // {
        //     lastHealthCheckMs = currentMillis;
        //     for (uint8_t i = 0; i < Task::numValues; i++)
        //     {
        //         if (tasks[i] == 0)
        //         {
        //             Serial.printf("Task %d is not operational\n", i);
        //             delay(WATCHDOG_TIMEOUT);
        //             // rp2040.reboot();
        //         }
        //     }
        //     // All tasks are operational
        //     // rp2040.wdt_reset();
        //     for (uint8_t i = 0; i < Task::numValues; i++)
        //     {
        //         tasks[i] = 0;
        //     }
        // }
        if (currentMillis - lastMemoryLog > LOG_MEMORY_FREQUENCY)
        {
            lastMemoryLog = currentMillis;
            Serial.printf("free heap: %d\n", rp2040.getFreeHeap());
        }
        
    }
}

/*

Each producer task will send a message to this watchdog task at a set interval.
Doing so will increment an array index corresponding to the task that sent the message.
Every x ms, the watchdog task will check the array to see if all tasks have sent a message.
If a task has not sent a message, the watchdog task will delay by the timeout
period thus causing the RP2040 to reset.

The data manager task does some dequeuing. It needs to specify ticks to wait
so that it can also frequently send a message to the watchdog task.

Likewise for all the consumer tasks.

*/