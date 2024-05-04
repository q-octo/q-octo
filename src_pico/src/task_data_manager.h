#pragma once

#include <Arduino.h>
#include <string>

class DataManager {
public:
  typedef enum {
    STATE_MOTORS, // taskMotors
    STATE_RC,     // taskReceiveFromRC
    ENABLE_WEB_SERVER,
    DISABLE_WEB_SERVER,
    TOGGLE_WEB_SERVER_ENABLED,
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
  } Type;

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
    uint16_t channels[16];
    bool failsafe;
  } RC;

  typedef struct {
    bool diagnosticsMode;
  } Diagnostics;

  typedef struct {
    float rpmL;
    float rpmR;
  } SetMotorSpeedIndividual;

  typedef struct {
    float rpm;
    float direction;
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
    OK,
    LOW_BATTERY,
    NO_TX_SIGNAL,
    MOTOR_ERROR,
    DISARMED,
  } Status;

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
    } as;
  } Message;

  typedef struct {
    Motors motors;
    Battery battery;
    RC rc;
    bool webServerEnabled;
    DisplayMessages displayMessages;
  } State;

  static void receiveMessage(const Message &message);

private:
  static void broadcastStateUpdate();

  static void setWebServerEnabled(bool enabled);


};
