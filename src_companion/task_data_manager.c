#include "task_data_manager.h"
#include "task_web_server.h"
#include "pico/stdlib.h"
#include <stdio.h>

QueueHandle_t dataManagerQueue;

void taskDataManager(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings

    printf("taskDataManager started\n");

    dataManagerQueue = xQueueCreate(10, sizeof(struct TaskMessage));
    if (dataManagerQueue == NULL)
    {
        printf("Failed to create dataManagerQueue\n");
        vTaskDelete(NULL);
    }

    struct TaskMessage message;

    for (;;)
    {
        if (xQueueReceive(dataManagerQueue, &message, portMAX_DELAY))
        {      
            printf("Received message: %s\n", message.Message);

            // Pass the message to the web server
            if (webServerQueue != NULL)
            {
                if (xQueueSend(webServerQueue, &message, 0) != pdPASS)
                {
                    printf("Failed to send message to web server\n");
                }
            }

        }
    }

}