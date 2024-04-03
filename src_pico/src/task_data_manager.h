#pragma once

#include "Arduino.h"
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

namespace TaskMessage
{
  typedef enum
  {
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
  } Type;

  typedef struct
  {
    struct
    {
      uint16_t temperature;
      float RPM;
      float torque;
      float position;
    } left;
    struct
    {
      uint16_t temperature;
      float RPM;
      float torque;
      float position;
    } right;

  } Motors;

  typedef struct
  {
    float voltage;
    float current;
    float fuel;
  } Battery;

  typedef struct
  {
    int16_t rssi;
    int16_t linkQuality;
    int16_t signalNoiseRatio;
    int16_t tx_power;
  } RC;

  typedef struct
  {
    bool diagnosticsMode;
  } Diagnostics;

  typedef struct
  {
    float rpmL;
    float rpmR;
  } SetMotorSpeedIndividual;

  typedef struct
  {
    float rpm;
    float direction;
  } SetMotorSpeedCombined;

  typedef struct
  {
    Type type;
    union
    {
      Motors motors;
      Battery battery;
      RC rc;
      Diagnostics diagnostics;
      SetMotorSpeedIndividual motorSpeedIndividual;
      SetMotorSpeedCombined motorSpeedCombined;
    };
  } Message;

}

extern QueueHandle_t dataManagerQueue;

void taskDataManager(void *pvParameters);
