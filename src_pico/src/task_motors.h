#pragma once

#include <Arduino.h> // Always include this first
#include "task_data_manager.h"
#include "xiaomi_cybergear_driver.h"

const uint8_t CYBERGEAR_CAN_ID_L = 0x7F; // 127
const uint8_t CYBERGEAR_CAN_ID_R = 0x7E; // 126
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
    UPDATE_SPEED_LIMIT,
    UPDATE_CURRENT_LIMIT,
    UPDATE_TORQUE_LIMIT,
    UPDATE_SPEED_KI,
    UPDATE_SPEED_KP,
  } MessageType;

  typedef struct {
    MessageType type;
    union {
      DataManager::SetMotorSpeedIndividual speedIndividual;
      DataManager::SetMotorSpeedCombined speedCombined;
      DataManager::CanMessage canMessage;
      DataManager::State state;
      float floatMotorParam;
    } as;
  } Message;

  static void init();

  static void receiveMessage(const Message &message);

  static void loop();

private:

  static void setSpeedIndividual(float speedL, float speedR);
  static void setSpeedCombined(float speed, float direction);
  static void broadcastStatusUpdate();
  static void setSpeedLimit(float speedLimit);
  static void setCurrentLimit(float currentLimit);
  static void setTorqueLimit(float torqueLimit);
  static void setSpeedKi(float speedKi);
  static void setSpeedKp(float speedKp);
  static void getSpeedLimit();
  static void getCurrentLimit();
  static void getTorqueLimit();
  static void getSpeedKi();
  static void getSpeedKp();

  static void initMotors();
  static void handleStateUpdate();
  
  static void checkCAN();

  static void debugPrintMotorStatus();
  

  static inline bool motorsEnabled = true;
  // static inline uint32_t lastStatusRequestMs = 0;
  static inline uint32_t lastStatusBroadcastMs = 0;
  static inline float maxSpeed = 0;
  static inline float maxTorque = 0;
  static inline float maxCurrent = 0;
  static inline float speedKi = 0;
  static inline float speedKp = 0;
  static inline DataManager::TimestampedMotorLimits leftMotorLimits{
    .last_max_speed_update = 0,
    .last_max_current_update = 0,
    .last_max_torque_update = 0,
    .last_speed_kp_update = 0,
    .last_speed_ki_update = 0,
  };
  static inline DataManager::TimestampedMotorLimits rightMotorLimits{
    .last_max_speed_update = 0,
    .last_max_current_update = 0,
    .last_max_torque_update = 0,
    .last_speed_kp_update = 0,
    .last_speed_ki_update = 0,
  };
  static inline unsigned long lastLeftMotorParameterResponseMicros = 0;
  static inline unsigned long lastRightMotorParameterResponseMicros = 0;
  // This needs to be sufficiently large otherwise we start talking over the motors
  static const int MOTOR_SPEED_UPDATE_INTERVAL_MICROS = 10'000;
  static const int MOTOR_COMMAND_DELAY_MICROS = 1000;
  static inline unsigned long lastSpeedUpdateMicros = 0;
  static inline float lastSpeedL = 0;
  static inline float lastSpeedR = 0;
  static inline bool pendingSpeedUpdate = false;
  static inline unsigned long lastParameterRequestMillis = 0;
  

  static inline XiaomiCyberGearDriver cybergearL =
          XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_L, MASTER_CAN_ID);
  static inline XiaomiCyberGearDriver cybergearR =
          XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_R, MASTER_CAN_ID);
};
