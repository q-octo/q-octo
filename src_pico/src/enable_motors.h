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