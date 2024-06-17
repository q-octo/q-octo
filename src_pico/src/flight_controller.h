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

#include "crsf.h"   
#include <SerialPIO.h>
#include "config.h"

class FlightController
{
public:
    static void init();
    static void loop();
private:
    static inline crsf_instance flConCrsfIns;
    static inline SerialPIO fcSerial = SerialPIO(SerialPIO::NOPIN, CFG_FLIGHT_CONTROLLER_UART_RX, 1024);
    static inline uint8_t flConSerialBuffer[128] = {0};
    static inline uint8_t crsfFrameIndex = 0;
    static inline uint8_t crsfFrameLength = 0;
    static inline uint8_t crsfCrcIndex = 0;


    static void onBatteryUpdate(const crsf_payload_battery_sensor_t battery);
};