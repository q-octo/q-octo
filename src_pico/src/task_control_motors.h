#pragma once
#include "task_data_manager.h"
#include "xiaomi_cybergear_driver.h"
#include <Arduino.h> // Always include this first

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127
const uint8_t MASTER_CAN_ID = 0x00;
extern XiaomiCyberGearDriver cybergearL;
extern XiaomiCyberGearDriver cybergearR;

namespace TaskControlMotors {

typedef enum {
  ENABLE,
  DISABLE,
  SET_SPEED_COMBINED,
  SET_SPEED_INDIVIDUAL, // Tank mode
} MessageType;

}

typedef struct {
  TaskControlMotors::MessageType type;
  union {
    TaskMessageSetMotorSpeedIndividual speedIndividual;
  };
} TaskControlMotorsMessage;

void taskControlMotors(void *pvParameters);
