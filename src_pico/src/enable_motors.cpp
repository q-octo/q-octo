#include "enable_motors.h"
#include "task_data_manager.h"
#include "config.h"


void EnableMotors::onTxFailsafed()
{
    failsafed = true;
    notify();
}

void EnableMotors::onTxFailsafeCleared()
{
    failsafed = false;
    notify();
}

void EnableMotors::onTxSwitchChange(bool on)
{
    armed = on;
    notify();
}

void EnableMotors::onBatteryVoltage(bool low)
{
    lowBattery = low;
    notify();
}

void EnableMotors::notify()
{
    if (isMotorEnabled() == lastMotorEnabled)
    {
        return;
    }
    lastMotorEnabled = isMotorEnabled();
    static DataManager::Message message;
    message.type = lastMotorEnabled 
        ? DataManager::Type::ENABLE_MOTORS 
        : DataManager::Type::DISABLE_MOTORS;
    DataManager::receiveMessage(message);
}

bool EnableMotors::isMotorEnabled()
{
    bool enabled = true;
    enabled = enabled && !failsafed;
    if (CFG_ENABLE_EXTRA_SWITCHES) {
        enabled = enabled && armed;
    }
    enabled = enabled && !lowBattery;
    return enabled;
}
