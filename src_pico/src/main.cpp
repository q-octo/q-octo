#include <Arduino.h>

#include <FreeRTOS.h> // Enables FreeRTOS and multicore support
#include <task.h>     // Enables FreeRTOS tasks

#include "can.h"
#include "task_control_motors.h"
#include "task_crsf.h"
#include "task_display.h"
#include "task_motors.h"

#define ENABLE_CAN 0
#define ENABLE_MOTORS 1
#define ENABLE_DISPLAY 1
// TODO fix bug that requires this to be enabled
#define DEBUG_LIST_TASKS 1
#define CORE_0 (1 << 0)
#define CORE_1 (1 << 1)

void printTaskStatus();

void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData,
                        bool extended);

void taskWatchdog(void *pvParameters);

void taskDebug(void *pvParameters);

void taskCAN(void *pvParameters);

TaskHandle_t watchdogHandle = nullptr;
TaskHandle_t crsfHandle = nullptr;
TaskHandle_t displayHandle = nullptr;
TaskHandle_t dataManagerHandle = nullptr;
TaskHandle_t canHandle = nullptr;
TaskHandle_t motorsHandle = nullptr;
TaskHandle_t controlMotorsHandle = nullptr;

std::map<eTaskState, const char *> eTaskStateName{{eReady, "Ready"},
                                                  {eRunning, "Running"},
                                                  {eBlocked, "Blocked"},
                                                  {eSuspended, "Suspended"},
                                                  {eDeleted, "Deleted"}};

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;          // Wait for serial connection to be established
  delay(1000); // Wait for a second
  Serial.println("Live on core 0");
#if ENABLE_CAN
  CanCommunication::init(onReceiveCanPacket);
#endif

  // Setup FreeRTOS tasks
  // Queue consumers need a higher priority than producers to avoid queue
  // overflow

  xTaskCreate(taskWatchdog, "taskWatchdog", configMINIMAL_STACK_SIZE, nullptr, 7, &watchdogHandle);
  vTaskCoreAffinitySet(watchdogHandle, CORE_0);

#if ENABLE_DISPLAY
  // Consumer (unless it is toggling diagnostics mode)
  xTaskCreate(taskDisplay, "taskDisplay", configMINIMAL_STACK_SIZE * (2^1), nullptr, 3, &displayHandle);
  vTaskCoreAffinitySet(displayHandle, CORE_0);
#endif

#if ENABLE_MOTORS
  xTaskCreate(taskControlMotors, "taskControlMotors", configMINIMAL_STACK_SIZE, nullptr, 1, &controlMotorsHandle);
  vTaskCoreAffinitySet(controlMotorsHandle, CORE_0);
#endif

  // Data Manager has a higher priority than producers (to prevent queue
  // overflows) and lower priority than consumers.
  xTaskCreate(taskDataManager, "taskDataManager", configMINIMAL_STACK_SIZE, nullptr, 2, &dataManagerHandle);
  vTaskCoreAffinitySet(dataManagerHandle, CORE_0);

  // Producer
  xTaskCreate(taskCRSF, "taskCRSF", configMINIMAL_STACK_SIZE, nullptr, 1, &crsfHandle);
  vTaskCoreAffinitySet(crsfHandle, CORE_0);
#if ENABLE_CAN
  xTaskCreate(taskCAN, "taskCAN", configMINIMAL_STACK_SIZE, NULL, 1, &canHandle);
  vTaskCoreAffinitySet(canHandle, CORE_0);
#endif
#if ENABLE_MOTORS
  xTaskCreate(taskMotors, "taskMotors", configMINIMAL_STACK_SIZE, nullptr, 1, &motorsHandle);
  vTaskCoreAffinitySet(motorsHandle, CORE_0);
#endif
}

// Handled by FreeRTOS
void loop() {
#if DEBUG_LIST_TASKS
  printTaskStatus();
  delay(5000);
#endif
}

void printTaskStatus() {
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
                  (int)pxTaskStatusArray[i].uxCurrentPriority,
                  pxTaskStatusArray[i].ulRunTimeCounter);
  }
  delete[] pxTaskStatusArray;
}

void taskWatchdog(void *pvParameters) {
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskWatchdog started");
  for (;;) {
    Serial.println("taskWatchdog tick");
    delay(pdMS_TO_TICKS(2000));
  }
}

void taskCAN(void *pvParameters) {
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskCAN started");
  for (;;) {
    CanCommunication::checkForPacket();
    delay(pdMS_TO_TICKS(1));
  }
}

void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData,
                        bool extended) {
  Serial.print("main.cpp: Received packet with id 0x");
  Serial.print(packetId, HEX);

  switch ((packetId & 0xFF00) >> 8) {
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
