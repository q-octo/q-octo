#pragma once

#include <Arduino.h>
#include <EEPROM.h>

class Storage
{
public:
    typedef struct State_s
    {
        float lowVoltageThreshold;
        float criticalVoltageThreshold;
        uint8_t batteryCount;
        uint8_t leftMotorFoldAngle;
        uint8_t rightMotorFoldAngle;
        uint8_t rssiThreshold;
        uint8_t linkQualityThreshold;
        bool startWebServerOnLaunch;
        float motorSpeedLimit;
        float motorCurrentLimit;
        float motorTorqueLimit;

        State_s()
            : lowVoltageThreshold(17.5f),      // 3.5V * 5
              criticalVoltageThreshold(17.0f), // 3.4V * 5
              batteryCount(4),
              leftMotorFoldAngle(0),
              rightMotorFoldAngle(0),
              rssiThreshold(105),
              linkQualityThreshold(70),
              startWebServerOnLaunch(false),
              motorSpeedLimit(2.0f),
              motorCurrentLimit(1.0f),
              motorTorqueLimit(1.0f)
        {
        }
    } State;

    static void init();
    static void save();

    static State &getState() { return state; }

private:
    static inline State state = State();
    static void notifyStateUpdate();
};