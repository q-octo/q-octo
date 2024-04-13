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
    ENABLE_WEB_SERVER,
    DISABLE_WEB_SERVER,
    ENABLE_MOTORS,
    DISABLE_MOTORS,
    SET_MOTOR_SPEED_COMBINED,
    SET_MOTOR_SPEED_INDIVIDUAL,
    // The RC signal was lost
    TX_LOST,
    // The RC signal was restored
    TX_RESTORED,
    CAN_MESSAGE_MOTOR_L,
    CAN_MESSAGE_MOTOR_R,
    CAN_MESSAGE_ROT_ENC_L,
    CAN_MESSAGE_ROT_ENC_R,
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
    uint8_t rssi;
    uint8_t linkQuality;
    int8_t signalNoiseRatio;
    uint16_t tx_power;
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
    uint32_t id;
    uint8_t* data;
  } CanMessage;


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
      CanMessage canMessage;
    } as;
  } Message;

}

extern QueueHandle_t dataManagerQueue;

void taskDataManager(void *pvParameters);
