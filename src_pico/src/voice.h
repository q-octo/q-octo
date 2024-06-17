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
#include <SerialPIO.h>
#include "config.h"

class Voice
{
public:
    typedef enum : uint8_t
    {
        EASTER_EGG = 1,
        OK,
        LOW_BATTERY,
        TX_LOST,
        TX_RESTORED,
        MOTOR_ERROR,
        DISARMED,
        POWER_ON,
        NAVIGATE_VIA_ONBOARD_COMPUTER,
        NAVIGATE_VIA_FLIGHT_CONTROLLER,
        NAVIGATE_VIA_RADIO,
        DASHBOARD_ENABLED,
        DASHBOARD_DISABLED,
        FOLDING_WHEELS,
        STAND_CLEAR,
    } Announcement;
    static void init();
    static void announce(Announcement announcement);

private:
    static inline SerialPIO voiceSerial = SerialPIO(CFG_VOICE_UART_TX, CFG_VOICE_UART_RX);

    static void setVolume(unsigned char vol);
    static void playTrackDocumented(uint16_t track);
    static void playTrack(unsigned char track);
};