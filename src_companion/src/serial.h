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
private:
    static bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);
    static inline FlatbufferSerialParser parser = FlatbufferSerialParser(Serial1, verifyIncomingFlatbuffer);
    static inline MessageCallback messageCallback;
    // 1024 is the default size, but it will grow automatically.
    static inline flatbuffers::FlatBufferBuilder fbb = flatbuffers::FlatBufferBuilder(1024);
};