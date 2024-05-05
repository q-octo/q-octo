#pragma once

#include <Arduino.h>
#include <EEPROM.h>

struct StorageState
{
    float lowVoltageThreshold = 17.5f;      // 3.5V * 5
    float criticalVoltageThreshold = 17.0f; // 3.4V * 5
    uint8_t batteryCount = 4;
    uint8_t leftMotorFoldAngle = 0;
    uint8_t rightMotorFoldAngle = 0;
    uint8_t rssiThreshold = 105;
    uint8_t linkQualityThreshold = 70;
    bool startWebServerOnLaunch = false;
    float motorSpeedLimit = 2.0f;
    float motorCurrentLimit = 1.0f;
    float motorTorqueLimit = 1.0f;
    float speedKp = 0.0f; // Default was 1
    float speedKi = 0.0f; // Default was 0.002
};

class Storage
{
public:
    typedef struct
    {
        StorageState state;
        uint32_t crc;
    } StateWithCRC;

    static void init();
    static void save();

    static StorageState &getState() { return state; }

private:
    static inline StorageState state;
    static inline StateWithCRC stateWithCRC = StateWithCRC();
    static void notifyStateUpdate();
    static bool isCrcValid();
    static uint32_t calculateCRC(StorageState &state);
};