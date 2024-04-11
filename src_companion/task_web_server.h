#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

struct WebMessage
{
  char Message[10];
};

void taskWebServer(void *pvParameters);

extern QueueHandle_t webServerQueue;