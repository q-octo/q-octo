#pragma once

static const int CFG_ENABLE_WATCHDOG = 0;
static const int CFG_ENABLE_CAN = 0;
static const int CFG_ENABLE_MOTORS = 1;
static const int CFG_ENABLE_RC = 0;
static const int CFG_ENABLE_POWER_MONITOR = 0;
static const int CFG_ENABLE_COMPANION = 1;
static const int CFG_ENABLE_ONBOARD_COMPUTER = 0;
static const int CFG_WAIT_FOR_USB_SERIAL = 0;

static const int CFG_DEBUG_LIST_TASKS = 0;
static const int CFG_DEBUG_WIPE_STORAGE = 1;

// Pins
static const int CFG_RC_UART_TX = 0;
static const int CFG_RC_UART_RX = 1;
static const int CFG_COMPANION_UART_TX = 2;
static const int CFG_COMPANION_UART_RX = 3;
static const int CFG_ONBOARD_COMPUTER_UART_TX = 4;
static const int CFG_ONBOARD_COMPUTER_UART_RX = 5;
static const int CFG_BTN_START_WEB_SERVER = 14;
static const int CFG_BTN_DISABLE_MOTORS = 15;