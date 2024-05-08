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
  static const uint8_t START_BYTE = 0xC8;

  bool parseMessage() { 
    while (serial.available()) {
      if (offset < 0) {
        if (serial.read() == START_BYTE) {
          offset = 0;
        }
        continue;
      }

      if (offset >= sizeof(buffer)) {
        Serial.printf("[ERROR] Buffer overflow. offset: %d\n", offset);
        reset();
        return false;
      }

      buffer[offset++] = serial.read();
      if (offset < 4) {
        continue;
      }

      if (offset == 4) {
        remainingBytes = flatbuffers::GetPrefixedSize(buffer);
        if (remainingBytes + 4 > sizeof(buffer)) {
          Serial.printf("[ERROR] Message too large: %d\n", remainingBytes);
          reset();
          return false;
        }
      }

      if (remainingBytes > 0) {
        remainingBytes--;
        continue;
      }

      flatbuffers::Verifier verifier(buffer, offset);
      reset();
      if (verify(verifier)) {
        return true;
      } else {
        Serial.println("[WARN] received invalid message, dropping remaining bytes.");
        return false;
      }
    }

    return false;
  }

private:
  HardwareSerial &serial;
  const VerifyCallback verify;
  int offset = -1;
  uint32_t remainingBytes = 0;

  void reset() {
    offset = -1;
    remainingBytes = 0;
  }
};
