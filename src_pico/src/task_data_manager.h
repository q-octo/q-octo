#pragma once

#include <Arduino.h>
#include <string>
#include "robot_state_generated.h"

class DataManager {
public:
  typedef enum {
    STATE_MOTORS, // taskMotors
    STATE_RC,     // taskReceiveFromRC
    ENABLE_WEB_SERVER,
    DISABLE_WEB_SERVER,
    DISPLAY_WEB_SERVER_BTN_PRESSED,
    ENABLE_MOTORS,
    DISABLE_MOTORS,
    SET_MOTOR_SPEED_COMBINED,       // taskReceiveFromRC (onboard computer in future)
    SET_MOTOR_SPEED_INDIVIDUAL,     // taskReceiveFromRC
    TX_LOST,                        // taskReceiveFromRC
    TX_RESTORED,                    // taskReceiveFromRC
    CAN_MESSAGE_MOTOR_L,            // taskCAN
    CAN_MESSAGE_MOTOR_R,            // taskCAN
    CAN_MESSAGE_ROT_ENC_L,          // taskCAN
    CAN_MESSAGE_ROT_ENC_R,          // taskCAN
    CAN_MESSAGE_POWER_MONITOR,      // taskCAN
    BATT_OK,                        // taskPowerMonitor
    BATT_VOLTAGE_LOW,               // taskPowerMonitor
    BATT_VOLTAGE_CRITICAL,          // taskPowerMonitor
    FOLD_WHEELS,                    // web server, taskReceiveFromRC
    DISPLAY_BUTTON_PRESSED,         // taskCompanionProducer
    STORAGE_UPDATE,
    DISPLAY_MESSAGES,
    BUTTON_DOWN,
    BUTTON_UP,
    UPDATE_MOTOR_SPEED_LIMIT, // Request to update a motor limit
    UPDATE_MOTOR_CURRENT_LIMIT,
    UPDATE_MOTOR_TORQUE_LIMIT,
    UPDATE_MOTOR_SPEED_KP,
    UPDATE_MOTOR_SPEED_KI,
    LEFT_MOTOR_PARAM_UPDATED,
    RIGHT_MOTOR_PARAM_UPDATED,
    TX_SWITCH_ARMED,
    TX_SWITCH_CONTROL_SOURCE,
    TX_SWITCH_WEB_SERVER,
  } Type;

  typedef struct {
    struct {
      float temperature;
      float RPM;
      float torque;
      float position;
    } left;
    struct {
      float temperature;
      float RPM;
      float torque;
      float position;
    } right;

  } Motors;

  typedef struct {
    float voltage;   // Volts
    float current;   // Amperes
    uint32_t fuel;   // mAh
    uint8_t percent; // 0-100
  } Battery;

  typedef struct {
    uint8_t rssi;
    uint8_t linkQuality;
    int8_t signalNoiseRatio;
    uint16_t tx_power;
    uint16_t *channels;
    bool failsafe;
  } RC;

  typedef struct {
    bool diagnosticsMode;
  } Diagnostics;

  typedef enum : uint8_t {
    MANUAL = 0,
    ONBOARD_COMPUTER,
    FLIGHT_CONTROLLER,
  } RobotControlSource;

  typedef struct {
    float leftSpeed;
    float rightSpeed;
    RobotControlSource controlSource;
  } SetMotorSpeedIndividual;

  typedef struct {
    float speed;
    float direction;
    RobotControlSource controlSource;
  } SetMotorSpeedCombined;

  typedef struct {
    uint32_t id;
    uint8_t *data;
    uint8_t len;
  } CanMessage;

  typedef enum {
    A, B
  } DisplayButton;

  typedef struct {
    std::string *message1;
    std::string *message2;
    std::string *message3;
    std::string *message4;
    std::string *message5;
    std::string *message6;
    std::string *message7;
  } DisplayMessages;

  typedef enum {
    WEB_SERVER,
    MOTORS,
  } PhysicalButton;

  typedef enum {
    OK = 0,
    LOW_BATTERY,
    NO_TX_SIGNAL,
    MOTOR_ERROR,
    DISARMED,
  } Status;

  typedef struct {
    float max_speed;
    float max_current;
    float max_torque;
    float speed_kp;
    float speed_ki;
    unsigned long last_max_speed_update;
    unsigned long last_max_current_update;
    unsigned long last_max_torque_update;
    unsigned long last_speed_kp_update;
    unsigned long last_speed_ki_update;
  } TimestampedMotorLimits;



  typedef struct {
    Type type;
    union {
      Motors motors;
      Battery battery;
      RC rc;
      Diagnostics diagnostics;
      SetMotorSpeedIndividual motorSpeedIndividual;
      SetMotorSpeedCombined motorSpeedCombined;
      CanMessage canMessage;
      float voltageThreshold;
      int batteryCount;
      DisplayButton displayButton;
      DisplayMessages displayMessages;
      PhysicalButton physicalButton;
      float floatMotorParam;
      TimestampedMotorLimits motorParams;
      bool txBinarySwitch;
      RobotControlSource txControlSourceSwitch;
    } as;
  } Message;

  typedef struct {
    Motors motors;
    Battery battery;
    RC rc;
    bool webServerEnabled;
    DisplayMessages displayMessages;
    TimestampedMotorLimits leftMotorLimits;
    TimestampedMotorLimits rightMotorLimits;
    RobotControlSource controlSource;
    // TODO set this 
    bool armed;
  } State; 

  static void receiveMessage(const Message &message);

  static State& getState() {
    return state;
  }

  static void init() {
    static std::string defaultMessage = "";
    state.displayMessages = {
      .message1 = &defaultMessage,
      .message2 = &defaultMessage,
      .message3 = &defaultMessage,
      .message4 = &defaultMessage,
      .message5 = &defaultMessage,
      .message6 = &defaultMessage,
      .message7 = &defaultMessage,
    };
    state.rc.channels = nullptr;
    state.controlSource = RobotControlSource::MANUAL;
    state.armed = false;
    // TODO does the rest of `state` need to be initialised here?
  }

private:
  static void broadcastStateUpdate();

  static void setWebServerEnabled(bool enabled);

  static inline State state{};


};
