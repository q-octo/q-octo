#pragma once

#include "Arduino.h"
#include "FreeRTOS.h"
#include "queue.h"


typedef enum {
  TASK_DISPLAY,
  TASK_CONTROL_MOTORS,
  TASK_SEND_TO_RC,
  TASK_DATA_MANAGER,
  TASK_RECEIVE_FROM_RC,
  TASK_CAN,
  TASK_MOTORS,
  numValues
} Task;

extern QueueHandle_t watchdogQueue;


void taskWatchdog(void *pvParameters);