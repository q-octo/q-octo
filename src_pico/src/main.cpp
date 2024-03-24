#include <Arduino.h>

#include <FreeRTOS.h> // Enables FreeRTOS and multicore support
#include <task.h>     // Enables FreeRTOS tasks

#include "can.h"
#include "xiaomi_cybergear_driver.h"
#include "pico/stdlib.h"
#include "task_crsf.h"
#include "task_display.h"
#include "task_motors.h"

#define ENABLE_CAN 0
#define DEBUG_LIST_TASKS 0
#define CORE_0 (1 << 0)
#define CORE_1 (1 << 1)

void printTaskStatus();
void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData, bool extended);
void taskWatchdog(void *pvParameters);
void taskDebug(void *pvParameters);
void taskCAN(void *pvParameters);

TaskHandle_t watchdogHandle = NULL;
TaskHandle_t crsfHandle = NULL;
TaskHandle_t displayHandle = NULL;
TaskHandle_t canHandle = NULL;
TaskHandle_t motorsHandle = NULL;

std::map<eTaskState, const char *> eTaskStateName{{eReady, "Ready"}, {eRunning, "Running"}, {eBlocked, "Blocked"}, {eSuspended, "Suspended"}, {eDeleted, "Deleted"}};

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;          // Wait for serial connection to be established
  delay(1000); // Wait for a second
  Serial.println("Live on core 0");
#if ENABLE_CAN
  CanCommunication::init(onReceiveCanPacket);
#endif

  // Setup FreeRTOS tasks
  xTaskCreate(taskWatchdog, "taskWatchdog", configMINIMAL_STACK_SIZE, NULL, 1, &watchdogHandle);
  vTaskCoreAffinitySet(watchdogHandle, CORE_0);
  xTaskCreate(taskCRSF, "taskCRSF", configMINIMAL_STACK_SIZE, NULL, 1, &crsfHandle);
  vTaskCoreAffinitySet(crsfHandle, CORE_0);
  xTaskCreate(taskDisplay, "taskDisplay", configMINIMAL_STACK_SIZE, NULL, 2, &displayHandle);
  vTaskCoreAffinitySet(displayHandle, CORE_1);
#if ENABLE_CAN
  xTaskCreate(taskCAN, "taskCAN", configMINIMAL_STACK_SIZE, NULL, 1, &canHandle);
  vTaskCoreAffinitySet(canHandle, CORE_0);
#endif
  xTaskCreate(taskMotors, "taskMotors", configMINIMAL_STACK_SIZE, NULL, 1, &motorsHandle);
  vTaskCoreAffinitySet(motorsHandle, CORE_0);
}

// Handled by FreeRTOS
void loop()
{
#if DEBUG_LIST_TASKS
  printTaskStatus();
  delay(5000);
#endif
}

void printTaskStatus()
{
  int tasks = uxTaskGetNumberOfTasks();
  TaskStatus_t *pxTaskStatusArray = new TaskStatus_t[tasks];
  unsigned long runtime;
  tasks = uxTaskGetSystemState(pxTaskStatusArray, tasks, &runtime);
  Serial.printf("# Tasks: %d\n", tasks);
  Serial.println("ID, NAME, STATE, PRIO, CYCLES");
  for (int i = 0; i < tasks; i++)
  {
    Serial.printf("%d: %-16s %-10s %d %lu\n", i, pxTaskStatusArray[i].pcTaskName, eTaskStateName[pxTaskStatusArray[i].eCurrentState], (int)pxTaskStatusArray[i].uxCurrentPriority, pxTaskStatusArray[i].ulRunTimeCounter);
  }
  delete[] pxTaskStatusArray;
}

void taskWatchdog(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  for (;;)
  {
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void taskCAN(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  for (;;)
  {
    CanCommunication::checkForPacket();
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData,
                        bool extended)
{
  Serial.print("main.cpp: Received packet with id 0x");
  Serial.print(packetId, HEX);

  switch ((packetId & 0xFF00) >> 8)
  {
  case CYBERGEAR_CAN_ID_L:
    cybergearL.process_message(packetData);
    break;
  case CYBERGEAR_CAN_ID_R:
    cybergearR.process_message(packetData);
    break;
  default:
    Serial.println("main.cpp: Received packet from unknown device");
    break;
  }
}
