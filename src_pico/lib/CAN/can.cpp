#include <Adafruit_MCP2515.h>
#include "can.h"

// https://github.com/adafruit/Adafruit_MCP2515/blob/4814941a02d663b82ecdb9548386f20b03ec108a/examples/MCP2515_CAN_ReceiverCallback/MCP2515_CAN_ReceiverCallback.ino#L32-L33
#define CS_PIN 20
#define INT_PIN 21

// Set CAN bus baud rate
#define CAN_BAUDRATE (1000000) // 1Mbit/s (determined by the motor)
Adafruit_MCP2515 mcp(CS_PIN);

PacketCallback _onPacketReceived;

void CanCommunication::init(PacketCallback onPacketReceived)
{
    if (!mcp.begin(CAN_BAUDRATE))
    {
        Serial.println("Starting CAN (MCP2515) failed");
        // Loop forever
        while (1)
            delay(10);
    }
    _onPacketReceived = onPacketReceived;
    Serial.println("CAN init complete");
}

void CanCommunication::checkForPacket()
{
    int packetSize = mcp.parsePacket();

    if (packetSize)
    {
        // received a packet
        Serial.print("Received ");

        bool extended = mcp.packetExtended();
        if (extended)
        {
            Serial.print("extended ");
        }

        if (mcp.packetRtr())
        {
            // Remote transmission request, packet contains no data
            Serial.print("RTR ");
        }

        Serial.print("packet with id 0x");
        Serial.print(mcp.packetId(), HEX);

        if (mcp.packetRtr())
        {
            Serial.print(" and requested length ");
            Serial.println(mcp.packetDlc());
        }
        else
        {
            Serial.print(" and length ");
            Serial.println(packetSize);
            uint8_t packetData[8] = {0};
            int i = 0;
            // only print packet data for non-RTR packets
            while (mcp.available())
            {
                packetData[i] = mcp.read();
                i++;
            }
            _onPacketReceived(packetSize, mcp.packetId(), packetData, extended);
            
            Serial.println();
        }

        Serial.println();
    }
}

/**
 * Sends a CAN packet with the specified ID and data.
 * Assumes that the data length is 8 bytes.
 *
 * @param id The ID of the CAN packet.
 * @param data Pointer to the data to be sent.
 */
void CanCommunication::sendCANPacket(uint32_t id, uint8_t *data)
{
    Serial.print("Sending packet with id 0x");
    Serial.println(id, HEX);
    // Remote transmission request (false for data frame, true for remote frame)
    bool rtr = false;
    // Data Length Code (how many bytes are being transmitted)
    int dlc = 8;
    if (!mcp.endPacket())
    {
        Serial.println("Failed to begin packet");
        return;
    }
    mcp.write(data, dlc);
    if (!mcp.endPacket())
    {
        Serial.println("Failed to end packet");
        return;
    }
}
