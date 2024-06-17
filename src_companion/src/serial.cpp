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
#include <Arduino.h>
#include "serial.h"

#define PIN_UART_TX 0
#define PIN_UART_RX 1

void QOctoSerial::init(MessageCallback callback) {
  messageCallback = callback;
  Serial1.setPinout(PIN_UART_TX, PIN_UART_RX);
  // Certainly needs to be larger than a single message ~108 bytes.
  // If messages are received faster than we process them then this will inevitably fill up and bytes will be lost.
  Serial1.setFIFOSize(4096);
  Serial1.begin(115200);
}

void QOctoSerial::loop() {
  if (parser.parseMessage()) {
    CompanionRxT message;
    GetSizePrefixedCompanionRx(parser.buffer)->UnPackTo(&message);
    if (messageCallback == nullptr) {
      Serial.println("[ERROR] QOctoSerial not initialised with a callback");
      return;
    }
    messageCallback(message);
  }
}

void QOctoSerial::serializeButtonPressed(Button button) {
  fbb.Reset();

    ButtonPressedT buttonPressed = ButtonPressedT();
    buttonPressed.button = button;

    auto buttonOffset = CreateButtonPressed(fbb, &buttonPressed);
    auto message = CreateCompanionTx(fbb, CompanionTxUnion::CompanionTxUnion_ButtonPressed, buttonOffset.Union());
    FinishSizePrefixedCompanionTxBuffer(fbb, message);
}

void QOctoSerial::sendButtonPressed(Button button) {
    Serial.println("Sending button pressed");
    serializeButtonPressed(button);
    
    Serial1.write(FlatbufferSerialParser::START_BYTE);
    Serial1.write(fbb.GetBufferPointer(), fbb.GetSize());
}

void QOctoSerial::sendUpdateMessage(uint8_t *data, size_t length) {
  Serial.println("Forwarding web server update message");
  fbb.Reset();

  UpdateT update;
  GetUpdate(data)->UnPackTo(&update);

  auto updateOffset = CreateUpdate(fbb, &update);
  auto message = CreateCompanionTx(fbb, CompanionTxUnion::CompanionTxUnion_Update, updateOffset.Union());
  FinishSizePrefixedCompanionTxBuffer(fbb, message);

  Serial1.write(FlatbufferSerialParser::START_BYTE);
  Serial1.write(fbb.GetBufferPointer(), fbb.GetSize());
}

bool QOctoSerial::verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier) {
  return SizePrefixedCompanionRxBufferHasIdentifier(parser.buffer) &&
         VerifySizePrefixedCompanionRxBuffer(verifier);
}
