#include "motor_enabled.h"
#include "task_data_manager.h"
#include "config.h"


void MotorEnabled::onTxFailsafed()
{
    failsafed = true;
    notify();
}

void MotorEnabled::onTxFailsafeCleared()
{
    failsafed = false;
    notify();
}

void MotorEnabled::onTxSwitchChange(bool on)
{
    armed = on;
    notify();
}

void MotorEnabled::onBatteryVoltage(bool low)
{
    lowBattery = low;
    notify();
}

void MotorEnabled::notify()
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

bool MotorEnabled::isMotorEnabled()
{
    bool enabled = true;
    enabled = enabled && !failsafed;
    if (CFG_ENABLE_EXTRA_SWITCHES) {
        enabled = enabled && armed;
    }
    enabled = enabled && !lowBattery;
    return enabled;
}
