#ifndef CAN_H
#define CAN_H

#include <Adafruit_MCP2515.h>

class CanCommunication
{
public:
    static void init(void (*callback)(int, long, uint8_t*)); // packetSize, packetId, packetData
    static void sendCANPacket(long id, uint8_t *data);
};

#endif // CAN_H