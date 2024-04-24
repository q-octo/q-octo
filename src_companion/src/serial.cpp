#include <Arduino.h>
#include "serial.h"

#define PIN_UART_TX 0
#define PIN_UART_RX 1

uint8_t serialBuffer[1024];

QOctoSerial::QOctoSerial()
{
    Serial1.setPinout(PIN_UART_TX, PIN_UART_RX);
    Serial1.begin(115200);
}

void QOctoSerial::loop()
{

    // Parse the serial data
    if (Serial1.available() > 0)
    {
        int len = Serial1.readBytes(serialBuffer, sizeof(serialBuffer));
    }

    // Send pending data here
}