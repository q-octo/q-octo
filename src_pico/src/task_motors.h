#pragma once

#include <Arduino.h> // Always include this first
#include "task_data_manager.h"
#include "xiaomi_cybergear_driver.h"

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127
const uint8_t MASTER_CAN_ID = 0x00;

class TaskControlMotors {

public:
  typedef enum {
    ENABLE,
    DISABLE,
    SET_SPEED_COMBINED,
    SET_SPEED_INDIVIDUAL, // Tank mode
    CAN_MESSAGE_MOTOR_L,
    CAN_MESSAGE_MOTOR_R,
    FOLD_WHEELS,
    STATE_UPDATE,
  } MessageType;

  typedef struct {
    MessageType type;
    union {
      DataManager::SetMotorSpeedIndividual speedIndividual;
      DataManager::SetMotorSpeedCombined speedCombined;
      DataManager::CanMessage canMessage;
      DataManager::State state;
    } as;
  } Message;

  static void init();

  static void receiveMessage(const Message &message);

  static void broadcastStatusUpdate();

private:

  static void setSpeedIndividual(float speedL, float speedR);
  static void setSpeedCombined(float speed, float direction);

  static void initMotors();
  static void handleStateUpdate();

  static void debugPrintMotorStatus();

  static inline bool motorsEnabled = true;
  static inline uint32_t lastStatusRequestMs = 0;
  static inline uint32_t lastStatusBroadcastMs = 0;
  static inline float maxSpeed;
  static inline float maxTorque;
  static inline float maxCurrent;

  static inline XiaomiCyberGearDriver cybergearL =
          XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_L, MASTER_CAN_ID);
  static inline XiaomiCyberGearDriver cybergearR =
          XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_R, MASTER_CAN_ID);
};
