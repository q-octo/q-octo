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
  // The RC signal was lost
  TX_LOST,
  // The RC signal was restored
  TX_RESTORED,
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
  int16_t rssi;
  int16_t linkQuality;
  int16_t signalNoiseRatio;
  int16_t tx_power;
} TaskMessageRC;

typedef struct {
  bool diagnosticsMode;
} TaskMessageDiagnostics;

typedef struct {
  float rpmL;
  float rpmR;
} TaskMessageSetMotorSpeedIndividual;

typedef struct {
  float rpm;
  float direction;
} TaskMessageSetMotorSpeedCombined;

typedef struct {
  TaskMessageType type;
  union {
    TaskMessageMotors motors;
    TaskMessageBattery battery;
    TaskMessageRC rc;
    TaskMessageDiagnostics diagnostics;
    TaskMessageSetMotorSpeedIndividual motorSpeedIndividual;
    TaskMessageSetMotorSpeedCombined motorSpeedCombined;
  };
} TaskMessage;

extern QueueHandle_t dataManagerQueue;

void taskDataManager(void *pvParameters);
