#include <Arduino.h>
#include "serial.h"

#define PIN_UART_TX 0
#define PIN_UART_RX 1

void QOctoSerial::init()
{
    Serial1.setPinout(PIN_UART_TX, PIN_UART_RX);
    Serial1.setFIFOSize(2048);
    Serial1.begin(115200);
}

void QOctoSerial::loop()
{
    if (parser.parseMessage())
    {
        Serial.println("We got a message");
    }
    // // Parse the serial data
    // if (Serial1.available() > 0)
    // {

    //     Serial.println("Hey mom, we got " + String(Serial1.available()) + " bytes");
    //     while (Serial1.available() > 0)
    //     {
    //         char c = Serial1.read();
    //         // Serial.print(c);
    //     }
    //     // One callback for state updates, one for enabling/disabling the web
    //     // server (bool enable parameter).
    //     // int len = Serial1.readBytes(serialBuffer, sizeof(serialBuffer));
    // }

    // // Send pending data here
}

bool QOctoSerial::verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier)
{
    return SizePrefixedCompanionRxBufferHasIdentifier(parser.buffer) &&
           VerifySizePrefixedCompanionRxBuffer(verifier);
}
