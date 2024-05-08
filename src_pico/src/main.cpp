#include "config.h"
#include <Arduino.h>
#include "storage.h"
#include "serial.h"

#include <FreeRTOS.h> // Enables FreeRTOS and multicore support
// #include <task.h>     // Enables FreeRTOS tasks

#include "task_motors.h"
#include "task_rc.h"
#include "task_watchdog.h"
#include "companion.h"
#include "computer.h"
#include "task_can.h"
#include "task_power_monitor.h"
#include "buttons.h"

#define CORE_0 (1 << 0)
#define CORE_1 (1 << 1)

void printTaskStatus();

void printHeapStats();

void initTasks();

// TaskHandle_t watchdogHandle = nullptr;

// std::map<eTaskState, const char *> eTaskStateName{{eReady,     "Ready"},
//                                                   {eRunning,   "Running"},
//                                                   {eBlocked,   "Blocked"},
//                                                   {eSuspended, "Suspended"},
//                                                   {eDeleted,   "Deleted"}};
uint32_t lastDebugListTasksMs = 0;

void setup() {
  Serial.begin(115200);
  if (CFG_WAIT_FOR_USB_SERIAL) {
    // Wait for serial connection to be established
    while (!Serial);
  }
  delay(2000); // Wait for motors to receive power.
  Serial.println("Live on core 0");

  Storage::init(); // Intentionally initialised first
  TaskCAN::init();
  TaskRC::init();
  TaskPowerMonitor::init();
  TaskControlMotors::init();
  Companion::init();
  Computer::init();
  Watchdog::init();
  Buttons::init();
  initTasks();
}

// Handled by FreeRTOS
void loop() {
  const uint32_t currentMillis = millis();
  if (CFG_DEBUG_LIST_TASKS) {
    if (currentMillis - lastDebugListTasksMs >= 5000) {
      lastDebugListTasksMs = currentMillis;
      printTaskStatus();
    }
  }

  TaskCAN::loop();
  TaskRC::loop();
  TaskControlMotors::loop();
  Companion::loop();
  Computer::loop();
  Buttons::loop();

  Watchdog::taskCompleted(Watchdog::Task::FAST_LOOP);
  Watchdog::loop();
  // It appears that a task labelled CORE0 runs this loop in a task
  // So if this loop never blocks, no other task on core 0 will run!
  //  vTaskDelay(pdMS_TO_TICKS(1));
}

void setup1() {
  SerialTask::init();
}

void loop1() { 
  SerialTask::loop();
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
