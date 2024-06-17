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
#include <FreeRTOS.h>
#include <queue.h>  
#include "config.h"


typedef enum {
    COMPANION,
    ONBOARD_COMPUTER,
} SerialTarget;

typedef struct {
    SerialTarget target;
    uint8_t *data;
    size_t size;
} SendSerialData;


class SerialTask
{
public:
    static void init();
    static void loop();
    static inline QueueHandle_t queue = nullptr;

private:
    static inline uint8_t sendBuffer[2048];
    static inline SerialPIO companionSerial =
          SerialPIO(CFG_COMPANION_UART_TX, SerialPIO::NOPIN, 1024);
    static inline SerialUART computerSerial = Serial2;
};