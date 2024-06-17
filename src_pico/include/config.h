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

static const int CFG_ENABLE_WATCHDOG = 1;
static const int CFG_ENABLE_CAN = 1;
static const int CFG_ENABLE_MOTORS = 1;
static const int CFG_ENABLE_RC = 1;
static const int CFG_ENABLE_POWER_MONITOR = 0;
static const int CFG_ENABLE_COMPANION = 1;
static const int CFG_ENABLE_ONBOARD_COMPUTER = 0;
static const int CFG_WAIT_FOR_USB_SERIAL = 0;

static const int CFG_DEBUG_WIPE_STORAGE = 0;
// Control source & armed switch don't work with the smaller transmitter yet
static const int CFG_ENABLE_EXTRA_SWITCHES = 0;

// Pins
static const int CFG_RC_UART_TX = 0;
static const int CFG_RC_UART_RX = 1;
static const int CFG_COMPANION_UART_TX = 2;
static const int CFG_COMPANION_UART_RX = 3;
static const int CFG_ONBOARD_COMPUTER_UART_TX = 4;
static const int CFG_ONBOARD_COMPUTER_UART_RX = 5;
static const int CFG_VOICE_UART_TX = 6;
static const int CFG_VOICE_UART_RX = 7;
// static const int CFG_FLIGHT_CONTROLLER_UART_TX = 8;
static const int CFG_FLIGHT_CONTROLLER_UART_RX = 9;
static const int CFG_BTN_START_WEB_SERVER = 14;
static const int CFG_BTN_DISABLE_MOTORS = 15;