#include <Arduino.h> // Always include this first
#include "FreeRTOS.h"
#include "task.h"

void taskCRSF(void *pvParameters)
{
    (void) pvParameters; //  To avoid warnings
    for (;;)
    {
        Serial.println("taskCRSF: Hello from taskCRSF");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for a second
    }
}