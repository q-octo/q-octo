#include <Arduino.h>
#include "serial.h"

#define PIN_UART_TX 0
#define PIN_UART_RX 1

void QOctoSerial::init(MessageCallback callback) {
  messageCallback = callback;
  Serial1.setPinout(PIN_UART_TX, PIN_UART_RX);
  // Certainly needs to be larger than a single message ~108 bytes.
  // If messages are received faster than we process them then this will inevitably fill up and bytes will be lost.
  Serial1.setFIFOSize(1024);
  Serial1.begin(115200);
}

void QOctoSerial::loop() {
  if (parser.parseMessage()) {
    CompanionRxT message;
    GetCompanionRx(parser.buffer)->UnPackTo(&message);
    if (messageCallback == nullptr) {
      Serial.println("[ERROR] QOctoSerial not initialised with a callback");
      return;
    }
    messageCallback(message);
  }
}

bool QOctoSerial::verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier) {
  return SizePrefixedCompanionRxBufferHasIdentifier(parser.buffer) &&
         VerifySizePrefixedCompanionRxBuffer(verifier);
}
