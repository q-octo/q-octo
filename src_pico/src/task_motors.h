#ifndef TASK_MOTORS_H
#define TASK_MOTORS_H

#include <Arduino.h> // Always include this first

#include "xiaomi_cybergear_driver.h"

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127
const uint8_t MASTER_CAN_ID = 0x00;
extern XiaomiCyberGearDriver cybergearL;
extern XiaomiCyberGearDriver cybergearR;

void taskMotors(void *pvParameters);

#endif // TASK_MOTORS_H