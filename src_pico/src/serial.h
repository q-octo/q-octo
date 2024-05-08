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