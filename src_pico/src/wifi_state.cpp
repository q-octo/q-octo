#include "wifi_state.h"
#include "storage.h"
#include "task_data_manager.h"

void WifiState::init() {}

void WifiState::onPhysicalSwitchChange(bool on)
{
    if (physicalSwitchOn == on && physicalSwitchInitialised)
    {
        return;
    }
    physicalSwitchOn = on;
    physicalSwitchInitialised = true;
    Serial.println("onPhysicalSwitchChange: " + String(on ? "on" : "off"));

    if (!switchesInitialised)
    {
        if (shouldInit())
        {
            onSwitchesInitialised();
            return;
        }
        Serial.println("[INFO] onPhysicalSwitchChange: switches not initialised yet");
        return;
    }

    if (on)
    {
        // Turn on if tx is also on or failsafed
        setWifiState(txOnOrFailsafed());
    }
    else
    {
        setWifiState(false);
    }
}

void WifiState::onTxSwitchChange(bool on)
{
    Serial.println("onTxSwitchChange: " + String(on ? "on" : "off"));
    if (txSwitchOn == on && txSwitchInitialised)
    {
        return;
    }
    txSwitchOn = on;
    txSwitchInitialised = true;

    Serial.println("2");
    if (!switchesInitialised)
    {
        if (shouldInit())
        {
            onSwitchesInitialised();
            return;
        }
        Serial.println("[INFO] onTxSwitchChange: switches not initialised yet");
        return;
    }
    Serial.println("3");

    if (on)
    {
        setWifiState(physicalSwitchOn);
    }
    else
    {
        setWifiState(false);
    }
}

void WifiState::onTxFailsafed()
{
    failsafed = true;
}

void WifiState::onTxFailsafeCleared()
{
    failsafed = false;
    /*
    It might be better to:
    - remove this
    - set txSwitchOn to false when failsafed
    - clear the failsafe via onTxSwitchChange
    */
}

void WifiState::onWifiButtonPress()
{
    const bool forcedOff = !physicalSwitchOn || !txSwitchOn;
    setWifiState(!forcedOff && !wifiEnabled);
}

void WifiState::setWifiState(bool on)
{
    if (wifiEnabled == on)
    {
        Serial.println("setWifiState: already " + String(on ? "enabled" : "disabled"));
        return;
    }
    wifiEnabled = on;
    Serial.println("setWifiState: " + String(wifiEnabled ? "enabled" : "disabled"));

    DataManager::Message message;
    message.type = on ? DataManager::Type::ENABLE_WEB_SERVER : DataManager::Type::DISABLE_WEB_SERVER;
    DataManager::receiveMessage(message);
}

void WifiState::onSwitchesInitialised()
{
    Serial.println("[INFO] onSwitchesInitialised");
    switchesInitialised = true;
    const bool startOnStartup = Storage::getState().startWebServerOnLaunch;
    setWifiState(startOnStartup && physicalSwitchOn && txSwitchOn);
}