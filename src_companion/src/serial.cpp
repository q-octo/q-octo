#include <Arduino.h>
#include "serial.h"

#define PIN_UART_TX 0
#define PIN_UART_RX 1

void QOctoSerial::init()
{
    
    Serial1.setPinout(PIN_UART_TX, PIN_UART_RX);
    Serial1.begin(115200);
}

void QOctoSerial::loop()
{
    // Parse the serial data
    if (Serial1.available() > 0)
    {
        Serial.println("Hey mom, wegot some bytes");
        // One callback for state updates, one for enabling/disabling the web 
        // server (bool enable parameter).
        // int len = Serial1.readBytes(serialBuffer, sizeof(serialBuffer));
    }

    // Send pending data here
}