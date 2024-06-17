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
#pragma once

#include <Arduino.h>
#include <EEPROM.h>

struct StorageState
{
    float lowVoltageThreshold = 17.5f;      // 3.5V * 5
    float criticalVoltageThreshold = 17.0f; // 3.4V * 5
    uint8_t batteryCount = 4;
    uint16_t leftMotorFoldAngle = 0;
    uint16_t rightMotorFoldAngle = 0;
    uint8_t rssiThreshold = 105;
    uint8_t linkQualityThreshold = 70;
    bool startWebServerOnLaunch = true;
    // Max = 30.0, only applies to position control mode!!
    float motorSpeedLimit = 15.0f;
    // max = 27.0
    float motorCurrentLimit = 1.0f;
    // Max = 12.5
    float motorTorqueLimit = 12.5f;
    float speedKp = 12.0f; 
    float speedKi = 12.0f;
    uint16_t radioReceiverTimeoutMillis = 2000;
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