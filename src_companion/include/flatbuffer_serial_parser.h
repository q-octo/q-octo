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

  uint8_t buffer[4096] = {0};

  bool parseMessage() {
    while (serial.available()) {
      if (offset >= sizeof(buffer)) {
        Serial.println("[ERROR] Buffer overflow.");
        dropRemainingBytes();
        return false;
      }

      buffer[offset++] = serial.read();
      if (offset < 4) {
        continue;
      }

      if (offset == 4) {
        remainingBytes = flatbuffers::GetPrefixedSize(buffer);
        if (remainingBytes + 4 > sizeof(buffer)) {
          Serial.println("[ERROR] Message too large.");
          dropRemainingBytes();
          return false;
        }
      }

      if (remainingBytes > 0) {
        remainingBytes--;
        continue;
      }

      flatbuffers::Verifier verifier(buffer, offset);
      if (verify(verifier)) {
        reset();
        return true;
      } else {
        Serial.println("[WARN] received invalid message, dropping remaining bytes.");
        dropRemainingBytes();
        return false;
      }
    }

    return false;
  }

private:
  HardwareSerial &serial;
  const VerifyCallback verify;
  int offset = 0;
  uint32_t remainingBytes = 0;

  void dropRemainingBytes() {
    while (serial.available()) {
      serial.read();
    }
    reset();
  }

  void reset() {
    offset = 0;
    remainingBytes = 0;
  }
};
