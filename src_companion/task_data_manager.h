#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

// Message structure
struct TaskMessage
{
  char Message[10];
};

extern QueueHandle_t dataManagerQueue;

void taskDataManager(void *pvParameters);