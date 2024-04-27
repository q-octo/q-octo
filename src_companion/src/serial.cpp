#include <Arduino.h>
#include "serial.h"

#define PIN_UART_TX 0
#define PIN_UART_RX 1

QOctoSerial::QOctoSerial()
{
}

void QOctoSerial::init(){
    Serial1.setPinout(PIN_UART_TX, PIN_UART_RX);
    Serial1.begin(115200);

    // Sleep for 10000ms
    Serial.println("Serial1 started");

}

void QOctoSerial::loop()
{

    Serial.println("Started loop");

    // Send Hello message on serial
    Serial1.println("Hello World");

    // Parse the serial data
    if (Serial1.available() > 0)
    {
        Serial.println("Entered if");
        // TODO(niall), read a single byte, this should specify the length of the message.
        // Then read up to that many bytes and fire a callback.
        // One callback for state updates, one for enabling/disabling the web 
        // server (bool enable parameter).
        int len = Serial1.readBytes(serialBuffer, sizeof(serialBuffer));

        // Print the output to the serial monitor
        for (int i = 0; i < len; i++)
        {
            Serial.print(serialBuffer[i]);
        }
        Serial.println();
    }

}