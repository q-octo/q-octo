#pragma once

#include <Arduino.h>
#include "flatbuffer_serial_parser.h"
#include "companion_rx_generated.h"
#include "companion_tx_generated.h"

using namespace fbs;

class QOctoSerial
{
public:
    static void init();
    static void loop();

private:
    static bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);
    static inline uint8_t serialBuffer[255];
    static inline FlatbufferSerialParser parser = FlatbufferSerialParser(Serial1, verifyIncomingFlatbuffer);
};