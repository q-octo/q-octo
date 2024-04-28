#include <Arduino.h>
#include "serial.h"


#define PIN_UART_TX 0
#define PIN_UART_RX 1


bool QOctoSerial::verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier)
{
    return SizePrefixedCompanionRxBufferHasIdentifier(fbSerialParser.buffer) &&
           VerifySizePrefixedCompanionRxBuffer(verifier);
}

void QOctoSerial::init()
{
    Serial1.setPinout(PIN_UART_TX, PIN_UART_RX);
    Serial1.begin(115200);
    Serial.println("Serial1 started");
    parseIncomingMessage();
}

void QOctoSerial::parseIncomingMessage(){
    if (!fbSerialParser.parseMessage())
    {
        return;
    }
    auto companionMessage = GetSizePrefixedCompanionTx(fbSerialParser.buffer);

    // Print the message
    Serial.println("Message received");
    Serial.println(companionMessage->message_type());
}

void QOctoSerial::loop()
{
    parseIncomingMessage();
}