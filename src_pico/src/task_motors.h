#pragma once

#include <Arduino.h> // Always include this first
#include "task_data_manager.h"
#include "xiaomi_cybergear_driver.h"

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127
const uint8_t MASTER_CAN_ID = 0x00;

namespace TaskControlMotors
{

  // public:
  typedef enum
  {
    ENABLE,
    DISABLE,
    SET_SPEED_COMBINED,
    SET_SPEED_INDIVIDUAL, // Tank mode
    CAN_MESSAGE_MOTOR_L,
    CAN_MESSAGE_MOTOR_R,
    FOLD_WHEELS,
  } MessageType;

  typedef struct
  {
    MessageType type;
    union
    {
      DataManager::SetMotorSpeedIndividual speedIndividual;
      DataManager::SetMotorSpeedCombined speedCombined;
      DataManager::CanMessage canMessage;
    } as;
  } Message;

  void init();

  void receiveMessage(const Message &message);

  // private:
  void broadcastStatusUpdate();

  void setSpeedIndividual(float speedL, float speedR);

  void initMotors();

  void debugPrintMotorStatus(); 
};
