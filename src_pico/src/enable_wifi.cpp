/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "enable_wifi.h"
#include "storage.h"
#include "task_data_manager.h"

void EnableWifi::init() {}

void EnableWifi::onPhysicalSwitchChange(bool on)
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

void EnableWifi::onTxSwitchChange(bool on)
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

void EnableWifi::onTxFailsafed()
{
    failsafed = true;
}

void EnableWifi::onTxFailsafeCleared()
{
    failsafed = false;
    /*
    It might be better to:
    - remove this
    - set txSwitchOn to false when failsafed
    - clear the failsafe via onTxSwitchChange
    */
}

void EnableWifi::onWifiButtonPress()
{
    const bool forcedOff = !physicalSwitchOn || !txSwitchOn;
    setWifiState(!forcedOff && !wifiEnabled);
}

void EnableWifi::setWifiState(bool on)
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

void EnableWifi::onSwitchesInitialised()
{
    Serial.println("[INFO] onSwitchesInitialised");
    switchesInitialised = true;
    const bool startOnStartup = Storage::getState().startWebServerOnLaunch;
    setWifiState(startOnStartup && physicalSwitchOn && txSwitchOn);
}