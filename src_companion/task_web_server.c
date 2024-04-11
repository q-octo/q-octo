#include "task_web_server.h"
#include "pico/stdlib.h"
#include <stdio.h>

QueueHandle_t webServerQueue;

void taskWebServer(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings

    printf("task_web_server started\n");

    webServerQueue = xQueueCreate(10, sizeof(struct WebMessage));
    if (webServerQueue == NULL)
    {
        printf("Failed to create webServerQueue\n");
        vTaskDelete(NULL);
    }

    struct WebMessage message;

    for (;;)
    {
        if (xQueueReceive(webServerQueue, &message, portMAX_DELAY))
        {      
            printf("Web message: %s\n", message.Message);
        }
    }

}