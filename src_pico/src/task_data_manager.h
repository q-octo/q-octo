#pragma once

#include "Arduino.h"
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

typedef enum {
  STATE_MOTORS,
  STATE_BATTERY,
  STATE_RC,
  STATE_DIAGNOSTICS,
  ENABLE_DIAGNOSTICS,
  DISABLE_DIAGNOSTICS,
  ENABLE_MOTORS,
  DISABLE_MOTORS,
  SET_MOTOR_SPEED_COMBINED,
  SET_MOTOR_SPEED_INDIVIDUAL,
} TaskMessageType;

typedef struct {
  struct {
    uint16_t temperature;
    float RPM;
    float torque;
    float position;
  } left;
  struct {
    uint16_t temperature;
    float RPM;
    float torque;
    float position;
  } right;

} TaskMessageMotors;

typedef struct {
  float voltage;
  float current;
  float fuel;
} TaskMessageBattery;

typedef struct {
  float rssi;
  float linkQuality;
} TaskMessageRC;

typedef struct {
  bool diagnosticsMode;
} TaskMessageDiagnostics;

typedef struct {
  int8_t rpmL;
  int8_t rpmR;
} TaskMessageSetMotorSpeedIndividual;

typedef struct {
  TaskMessageType type;
  union {
    TaskMessageMotors motors;
    TaskMessageBattery battery;
    TaskMessageRC rc;
    TaskMessageDiagnostics diagnostics;
    TaskMessageSetMotorSpeedIndividual motorSpeedIndividual;
  };
} TaskMessage;

extern QueueHandle_t dataManagerQueue;

void taskDataManager(void *pvParameters);
