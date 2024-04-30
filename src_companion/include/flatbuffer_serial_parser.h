#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <functional>
#include <utility>
#include "flatbuffers/flatbuffers.h"

class FlatbufferSerialParser {
public:
  typedef bool (*VerifyCallback)(flatbuffers::Verifier &verifier);

  FlatbufferSerialParser(HardwareSerial &serial, const VerifyCallback verify) : serial(serial), verify(verify) {
  }

  uint8_t buffer[1024] = {0};

  bool parseMessage() {
    int offset = 0;
    uint32_t remainingBytes = 0;
    const int bytesInSerialFifo = serial.available();

    if (bytesInSerialFifo) {
      // print message length
    }

    while (serial.available()) {
      if (offset >= sizeof(buffer)) {
        Serial.println("[ERROR] Buffer overflow.");
        dropRemainingBytes();
      }

      buffer[offset++] = serial.read();
      if (offset < 4) {
        continue;
      }

      if (offset == 4) {
        remainingBytes = flatbuffers::GetPrefixedSize(buffer);
        Serial.println("FBS message length: " + String(remainingBytes) + " bytes.");
        const int remainingBytesInSerialFifo = serial.available();
        Serial.print("Bytes in serial queue: ");
        Serial.println(serial.available());
        if (remainingBytes > remainingBytesInSerialFifo) {
          const int missingBytes = remainingBytes - remainingBytesInSerialFifo;
          // Serial.printf("[WARN] missing %d bytes\n", missingBytes);
          delayMicroseconds(100 * missingBytes);
          const int bytesReceivedFromDelay = serial.available() - remainingBytesInSerialFifo;
          // Serial.printf("[INFO] received %d bytes from delay\n", bytesReceivedFromDelay);
          if (missingBytes > bytesReceivedFromDelay) {
            const int newMissingBytes = missingBytes - bytesReceivedFromDelay;
            Serial.printf("[WARN] missing %d bytes after delay\n", newMissingBytes);
          }
        }

      }

      if (remainingBytes > 0) {
        remainingBytes--;
        continue;
      }


      flatbuffers::Verifier verifier(buffer, offset);
      if (verify(verifier)) {
        return true;
      } else {
        Serial.println("[WARN] received invalid message, dropping remaining bytes.");
        dropRemainingBytes();
        return false;
      }
    }

    if (remainingBytes > 0) {
      Serial.println("[WARN] expected more bytes but none available.");
      Serial.print("Remaining bytes: ");
      Serial.println(remainingBytes);
      Serial.print("Offset: ");
      Serial.println(offset);
    }

    return false;
  }

private:
  HardwareSerial &serial;
  const VerifyCallback verify;

  void dropRemainingBytes() {
    while (serial.available()) {
      serial.read();
    }
  }
};
