#include "FreeRTOS.h"
#include "task.h"
#include <Arduino.h>

void taskCRSF(void *pvParameters)
{
    for (;;)
    {
        Serial.println("taskCRSF: Hello from taskCRSF");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for a second
    }
}