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
#include "flatbuffer_serial_parser.h"
#include "companion_rx_generated.h"
#include "companion_tx_generated.h"

using namespace fbs;

class QOctoSerial
{
public:
    typedef void (*MessageCallback)(const CompanionRxT &message);
    static void init(MessageCallback callback);
    static void loop();
    static void serializeButtonPressed(fbs::Button button);
    static void sendButtonPressed(fbs::Button button);
    static void sendUpdateMessage(uint8_t *data, size_t length);
private:
    static bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);
    static inline FlatbufferSerialParser parser = FlatbufferSerialParser(Serial1, verifyIncomingFlatbuffer);
    static inline MessageCallback messageCallback;
    // 1024 is the default size, but it will grow automatically.
    static inline flatbuffers::FlatBufferBuilder fbb = flatbuffers::FlatBufferBuilder(1024);
};