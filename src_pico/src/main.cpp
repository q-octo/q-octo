#include "config.h"
#include <Arduino.h>

//#include <FreeRTOS.h> // Enables FreeRTOS and multicore support
//#include <task.h>     // Enables FreeRTOS tasks

#include "task_motors.h"
#include "task_rc.h"
#include "task_watchdog.h"
#include "companion.h"
#include "computer.h"
#include "task_can.h"
#include "task_power_monitor.h"

#define CORE_0 (1 << 0)
#define CORE_1 (1 << 1)

void printTaskStatus();

void printHeapStats();

void initTasks();

//TaskHandle_t watchdogHandle = nullptr;

//std::map<eTaskState, const char *> eTaskStateName{{eReady,     "Ready"},
//                                                  {eRunning,   "Running"},
//                                                  {eBlocked,   "Blocked"},
//                                                  {eSuspended, "Suspended"},
//                                                  {eDeleted,   "Deleted"}};
uint32_t lastDebugListTasksMs = 0;

void setup() {
  Serial.begin(115200);
  // Wait for serial connection to be established
  while (!Serial);
  delay(1000); // Wait for a second
  Serial.println("Live on core 0");
  TaskCAN::init();
  TaskRC::init();
  TaskPowerMonitor::init();
  TaskControlMotors::init();
  Companion::init();
  Computer::init();
  Watchdog::init();
  initTasks();
}

// Handled by FreeRTOS
void loop() {
#if CFG_DEBUG_LIST_TASKS
  const uint32_t currentMillis = millis();
  if (currentMillis - lastDebugListTasksMs >= 5000)
  {
    lastDebugListTasksMs = currentMillis;
    printTaskStatus();
  }
#endif

  TaskCAN::loop();
  TaskRC::loop();
  // TODO this should run slower
  TaskControlMotors::broadcastStatusUpdate();
  Companion::loop();
  Computer::loop();

  Watchdog::taskCompleted(Watchdog::Task::FAST_LOOP);
  Watchdog::loop();
  // It appears that a task labelled CORE0 runs this loop in a task
  // So if this loop never blocks, no other task on core 0 will run!
//  vTaskDelay(pdMS_TO_TICKS(1));
}


void initTasks() {
  // Setup FreeRTOS tasks
  // Queue consumers need a higher priority than producers to avoid queue
  // overflow

  // We likely only needed this for the controlMotor task?
//  const uint32_t stackSize = configMINIMAL_STACK_SIZE * 2;

  // Remaining 'tasks'
  // wifi switch, motor off switch, voice module, flight controller


//  xTaskCreate(taskWatchdog, "watchdog", stackSize, nullptr, 4, &watchdogHandle);
//  vTaskCoreAffinitySet(watchdogHandle, CORE_0);
}

void printTaskStatus() {
  /*
  uint32_t tasks = uxTaskGetNumberOfTasks();
  auto *pxTaskStatusArray = new TaskStatus_t[tasks];
  unsigned long runtime;
  tasks = uxTaskGetSystemState(pxTaskStatusArray, tasks, &runtime);
  Serial.printf("# Tasks: %lu\n", tasks);
  Serial.println("ID, NAME, STATE, PRIO, CYCLES");
  for (int i = 0; i < tasks; i++) {
    Serial.printf("%d: %-16s %-10s %d %lu\n", i,
                  pxTaskStatusArray[i].pcTaskName,
                  eTaskStateName[pxTaskStatusArray[i].eCurrentState],
                  (int) pxTaskStatusArray[i].uxCurrentPriority,
                  pxTaskStatusArray[i].ulRunTimeCounter);
  }
  delete[] pxTaskStatusArray;
   */
}
