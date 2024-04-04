#include <Arduino.h>

#include <FreeRTOS.h> // Enables FreeRTOS and multicore support
#include <task.h>     // Enables FreeRTOS tasks

#include <WiFi.h>
#include "can.h"
#include "task_control_motors.h"
#include "task_rc.h"
#include "task_display.h"
#include "task_motors.h"
#include "web_server.h"

#define ENABLE_CAN 1
#define ENABLE_MOTORS 1
#define ENABLE_DISPLAY 1
#define ENABLE_WEB_SERVER 1
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
TaskHandle_t receiveFromRCHandle = nullptr;
TaskHandle_t sendToRCHandle = nullptr;
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
uint32_t lastDebugListTasksMs = 0;


void setup()
{
  Serial.begin(115200);
  // Wait for serial connection to be established
  while (!Serial);
  delay(1000); // Wait for a second
  Serial.println("Live on core 0");
#if ENABLE_CAN
  CanCommunication::init(onReceiveCanPacket);
#endif

  // Setup FreeRTOS tasks
  // Queue consumers need a higher priority than producers to avoid queue
  // overflow

  xTaskCreate(taskWatchdog, "watchdog", configMINIMAL_STACK_SIZE, nullptr, 7, &watchdogHandle);
  vTaskCoreAffinitySet(watchdogHandle, CORE_1);

#if ENABLE_DISPLAY
  // Consumer (unless it is toggling diagnostics mode)
  xTaskCreate(taskDisplay, "display", configMINIMAL_STACK_SIZE * (2^1), nullptr, 3, &displayHandle);
  vTaskCoreAffinitySet(displayHandle, CORE_1);
#endif

#if ENABLE_MOTORS
  xTaskCreate(taskControlMotors, "ctrlMotors", configMINIMAL_STACK_SIZE, nullptr, 3, &controlMotorsHandle);
  vTaskCoreAffinitySet(controlMotorsHandle, CORE_1);
#endif

  xTaskCreate(taskSendToRC, "sndToRC", configMINIMAL_STACK_SIZE, nullptr, 3, &sendToRCHandle);
  vTaskCoreAffinitySet(sendToRCHandle, CORE_1);

  // Data Manager has a higher priority than producers (to prevent queue
  // overflows) and lower priority than consumers.
  xTaskCreate(taskDataManager, "dataManager", configMINIMAL_STACK_SIZE, nullptr, 2, &dataManagerHandle);
  vTaskCoreAffinitySet(dataManagerHandle, CORE_1);

  // Producer
  xTaskCreate(taskReceiveFromRC, "recFromRC", configMINIMAL_STACK_SIZE, nullptr, 1, &receiveFromRCHandle);
  vTaskCoreAffinitySet(receiveFromRCHandle, CORE_1);
#if ENABLE_CAN
  xTaskCreate(taskCAN, "can", configMINIMAL_STACK_SIZE, NULL, 1, &canHandle);
  vTaskCoreAffinitySet(canHandle, CORE_1);
#endif
#if ENABLE_MOTORS
  // Producer
  xTaskCreate(taskMotors, "motors", configMINIMAL_STACK_SIZE, nullptr, 1, &motorsHandle);
  vTaskCoreAffinitySet(motorsHandle, CORE_1);
#endif
}

// Handled by FreeRTOS
void loop()
{
#if DEBUG_LIST_TASKS
  const uint32_t currentMillis = millis();
  if (currentMillis - lastDebugListTasksMs >= 5000)
  {
    lastDebugListTasksMs = currentMillis;
    printTaskStatus();
  }
#endif
#if ENABLE_WEB_SERVER
  WSWebServer::start();
  WSWebServer::loop();
#endif
  // It appears that a task labelled CORE0 runs this loop in a task
  // So if this loop never blocks, no other task on core 0 will run!
  delay(1);
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

uint32_t getTotalHeap(void) {
    extern char __StackLimit, __bss_end__;
    return &__StackLimit - &__bss_end__;
}

uint32_t getFreeHeap(void) {
    struct mallinfo m = mallinfo();
    return getTotalHeap() - m.uordblks;
}

void taskWatchdog(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskWatchdog started");
  for (;;)
  {
    Serial.printf("taskWatchdog tick, free heap: %lu\n", getFreeHeap());
    delay(5000);
  }
}

void taskCAN(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskCAN started");
  for (;;)
  {
    CanCommunication::checkForPacket();
    delay(1);
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
