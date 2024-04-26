#include "config.h"
#include <Arduino.h>

#include <FreeRTOS.h> // Enables FreeRTOS and multicore support
#include <task.h>     // Enables FreeRTOS tasks

#include "can.h"
#include "task_motors.h"
#include "task_rc.h"
#include "task_can.h"
#include "task_watchdog.h"
#include "task_power_monitor.h"
#include "companion.h"
#include "computer.h"

#define CORE_0 (1 << 0)
#define CORE_1 (1 << 1)

void printTaskStatus();
void printHeapStats();
void initTasks();

TaskHandle_t watchdogHandle = nullptr;
TaskHandle_t receiveFromRCHandle = nullptr;
TaskHandle_t sendToRCHandle = nullptr;
TaskHandle_t dataManagerHandle = nullptr;
TaskHandle_t canHandle = nullptr;
TaskHandle_t motorsHandle = nullptr;
TaskHandle_t controlMotorsHandle = nullptr;
TaskHandle_t companionTask = nullptr;
TaskHandle_t computerTask = nullptr;

std::map<eTaskState, const char *> eTaskStateName{{eReady, "Ready"},
                                                  {eRunning, "Running"},
                                                  {eBlocked, "Blocked"},
                                                  {eSuspended, "Suspended"},
                                                  {eDeleted, "Deleted"}};
uint32_t lastDebugListTasksMs = 0;

void setup()
{
  Serial.begin(115200);
  // Wait for serial connection to be established
  while (!Serial)
    ;
  delay(1000); // Wait for a second
  Serial.println("Live on core 0");
#if CFG_ENABLE_CAN
  // IMPORTANT that this occurs outside of a FreeRTOS task (maybe so that we
  // don't send a CAN message before CAN is initialized?)
  CanCommunication::init(onReceiveCanPacket);
#endif
  initTasks();
}

// Handled by FreeRTOS
void loop()
{
#if CFG_DEBUG_LIST_TASKS
  const uint32_t currentMillis = millis();
  if (currentMillis - lastDebugListTasksMs >= 5000)
  {
    lastDebugListTasksMs = currentMillis;
    printTaskStatus();
  }
#endif
#if CFG_ENABLE_WEB_SERVER
  WSWebServer::loop();
#endif
  // It appears that a task labelled CORE0 runs this loop in a task
  // So if this loop never blocks, no other task on core 0 will run!
  vTaskDelay(pdMS_TO_TICKS(1));
}

void initTasks()
{
  // Setup FreeRTOS tasks
  // Queue consumers need a higher priority than producers to avoid queue
  // overflow

  // We likely only needed this for the controlMotor task?
  const uint32_t stackSize = configMINIMAL_STACK_SIZE * 2;

  // Remaining 'tasks'
  // wifi switch, motor off switch, voice module, flight controller

  xTaskCreate(taskWatchdog, "watchdog", stackSize, nullptr, 4, &watchdogHandle);
  vTaskCoreAffinitySet(watchdogHandle, CORE_0);
  xTaskCreate(taskControlMotors, "ctrlMotors", stackSize, nullptr, 3, &controlMotorsHandle);
  vTaskCoreAffinitySet(controlMotorsHandle, CORE_0);
  xTaskCreate(taskSendToRC, "sndToRC", stackSize, nullptr, 3, &sendToRCHandle);
  vTaskCoreAffinitySet(sendToRCHandle, CORE_0);
  // Data Manager has a higher priority than producers (to prevent queue
  // overflows) and lower priority than consumers.
  xTaskCreate(taskDataManager, "dataManager", stackSize, nullptr, 2, &dataManagerHandle);
  vTaskCoreAffinitySet(dataManagerHandle, CORE_0);
  TaskPowerMonitor::init();
#if CFG_ENABLE_RC
  xTaskCreate(taskReceiveFromRC, "recFromRC", stackSize, nullptr, 1, &receiveFromRCHandle);
  vTaskCoreAffinitySet(receiveFromRCHandle, CORE_0);
#endif
#if CFG_ENABLE_CAN
  xTaskCreate(taskCAN, "can", stackSize, nullptr, 1, &canHandle);
  vTaskCoreAffinitySet(canHandle, CORE_0);
#endif
#if CFG_ENABLE_MOTORS
  // Producer
  xTaskCreate(taskMotors, "motors", stackSize, nullptr, 1, &motorsHandle);
  vTaskCoreAffinitySet(motorsHandle, CORE_0);
#endif
#if CFG_ENABLE_COMPANION
  xTaskCreate(Companion::task, "cpnProducer", stackSize, nullptr, 1, &companionTask);
  vTaskCoreAffinitySet(companionTask, CORE_0);
#endif
#if CFG_ENABLE_ONBOARD_COMPUTER
  xTaskCreate(Computer::task, "cptrProducer", stackSize, nullptr, 1, &computerTask);
  vTaskCoreAffinitySet(computerTask, CORE_0);
#endif
}

void printTaskStatus()
{
  uint32_t tasks = uxTaskGetNumberOfTasks();
  auto *pxTaskStatusArray = new TaskStatus_t[tasks];
  unsigned long runtime;
  tasks = uxTaskGetSystemState(pxTaskStatusArray, tasks, &runtime);
  Serial.printf("# Tasks: %lu\n", tasks);
  Serial.println("ID, NAME, STATE, PRIO, CYCLES");
  for (int i = 0; i < tasks; i++)
  {
    Serial.printf("%d: %-16s %-10s %d %lu\n", i,
                  pxTaskStatusArray[i].pcTaskName,
                  eTaskStateName[pxTaskStatusArray[i].eCurrentState],
                  (int)pxTaskStatusArray[i].uxCurrentPriority,
                  pxTaskStatusArray[i].ulRunTimeCounter);
  }
  delete[] pxTaskStatusArray;
}
