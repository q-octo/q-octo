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
