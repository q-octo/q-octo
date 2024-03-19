#ifndef CAN_H
#define CAN_H

#include <Adafruit_MCP2515.h>

class CanCommunication
{
public:
    static void init(void (*callback)(int, uint32_t, uint8_t*)); // packetSize, packetId, packetData
    static void sendCANPacket(uint32_t id, uint8_t *data);
};


extern void check();
#endif // CAN_H