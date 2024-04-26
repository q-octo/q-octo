#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <functional>
#include <utility>
#include "flatbuffers/flatbuffers.h"

class FlatbufferSerialParser {
public:
  using VerifyCallback = std::function<bool(flatbuffers::Verifier &)>;

  FlatbufferSerialParser(HardwareSerial &serial, const VerifyCallback &verify) : serial(serial), verify(verify) {}

  uint8_t buffer[1024] = {0};

  bool parseMessage() {
    int offset = 0;
    uint32_t remainingBytes = 0;

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
      }

      if (remainingBytes > 0) {
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
    }
    return false;
  }

private:
  HardwareSerial &serial;
  const VerifyCallback &verify;

  void dropRemainingBytes() {
    while (serial.available()) {
      serial.read();
    }
  }
};
