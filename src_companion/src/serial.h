#pragma once

#include "flatbuffers/flatbuffers.h"
#include "flatbuffer_serial_parser.h"
#include "companion_rx_generated.h"
#include "companion_tx_generated.h"

using namespace fbs;

class QOctoSerial
{
public:
    static void init();
    static bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);
    static void loop();
    static void parseIncomingMessage();

private:
    static inline uint8_t serialBuffer[255];
    static inline SerialUART fbSerial = Serial1;
    static inline FlatbufferSerialParser fbSerialParser = FlatbufferSerialParser(fbSerial, verifyIncomingFlatbuffer);
};