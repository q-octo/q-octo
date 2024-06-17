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

class EnableWifi
{
public:
    static void init();
    static void onPhysicalSwitchChange(bool on);
    static void onTxSwitchChange(bool on);
    static void onTxFailsafed();
    static void onTxFailsafeCleared();
    static void onWifiButtonPress();
    
private:
    static void setWifiState(bool on);
    static bool txOnOrFailsafed() { return txSwitchOn || failsafed; }
    static bool shouldInit() { return !switchesInitialised && txSwitchInitialised && physicalSwitchInitialised; }
    static void onSwitchesInitialised();
    static inline bool physicalSwitchOn = false;
    static inline bool txSwitchOn = false;
    static inline bool failsafed = false;
    static inline bool wifiEnabled = false;
    static inline bool switchesInitialised = false;
    static inline bool txSwitchInitialised = false;
    static inline bool physicalSwitchInitialised = false;
};