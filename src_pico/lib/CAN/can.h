#pragma once
#include <Adafruit_MCP2515.h>

typedef void (*PacketCallback)(uint8_t packetLength, uint32_t packetId, uint8_t *packetData, bool extended);

class CanCommunication
{
public:
    static void init(PacketCallback callback);
    static void checkForPacket();
    static void sendCANPacket(uint32_t id, uint8_t *data);
};
