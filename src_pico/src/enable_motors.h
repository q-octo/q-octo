#pragma once

/*

Decides whether or not the motor is enabled based on:
* TX failsafe
* Arm switch
*/

class EnableMotors {
public:
    static void onTxFailsafed();
    static void onTxFailsafeCleared();
    static void onTxSwitchChange(bool armed);
    static void onBatteryVoltage(bool low);
private:
    static inline bool failsafed = false;
    static inline bool armed = false;
    static inline bool lowBattery = false;
    static inline bool lastMotorEnabled = false;
    
    static void notify();
    static bool isMotorEnabled();
};