#include <Adafruit_MCP2515.h>
#include "can.h"

// https://github.com/adafruit/Adafruit_MCP2515/blob/4814941a02d663b82ecdb9548386f20b03ec108a/examples/MCP2515_CAN_ReceiverCallback/MCP2515_CAN_ReceiverCallback.ino#L32-L33
#define CS_PIN 20
#define INT_PIN 21

// Set CAN bus baud rate
#define CAN_BAUDRATE (250000)
Adafruit_MCP2515 mcp(CS_PIN);

void (*onExtendedPacketReceived)(int, uint32_t, uint8_t *);


void check() {
    Serial.println("Checking");
    if (mcp.parsePacket()) {
        Serial.println("Received packet!!");
    } else {
        Serial.println("No packet received");
    }

}

void onReceive(int packetSize)
{
    // received a packet
    Serial.print("Received ");

    if (mcp.packetExtended())
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
            Serial.print((char)mcp.read());
        }
        onExtendedPacketReceived(packetSize, mcp.packetId(), packetData);
        Serial.println();
    }

    Serial.println();
}

void CanCommunication::init(void (*callback)(int, uint32_t, uint8_t *)) // packetLength, packetId, packetData
{
    if (!mcp.begin(CAN_BAUDRATE))
    {
        Serial.println("Starting CAN (MCP2515) failed!");
        // Loop forever
        while (1) delay(10);
    }
    Serial.println("MCP2515 Initialized Successfully!");

    // mcp.onReceive(INT_PIN, onReceive);


    // ACK our own transmitted packets
    // TODO we shouldn't do this but the motor isn't ACKing our packets
    // if (mcp.loopback() == 0) {
    //     Serial.println("MCP2515 Loopback mode failed");
    // } else {
    //     Serial.println("MCP2515 Loopback mode set");
    // }

    onExtendedPacketReceived = callback;
    Serial.println("CAN init complete");
}

void CanCommunication::sendCANPacket(uint32_t id, uint8_t *data)
{
    if (mcp.parsePacket()) {
        Serial.println("Tried to send packet but a receive packet was pending");
        return;
    }
    Serial.print("Sending packet with id 0x");
    Serial.println(id, HEX);
    // Remote transmission request (false for data frame, true for remote frame)
    bool rtr = false;
    // Data Length Code (how many bytes are being transmitted)
    int dlc = 8;
    int beginPacketResult = mcp.beginExtendedPacket(id, dlc, rtr);
    if (beginPacketResult == 0)
    {
        Serial.println("Failed to begin packet");
        return;
    }
    Serial.println("Packet begun");
    mcp.write(data, dlc);
    // for (int i = 0; i < dlc; i++)
    // {
    //     mcp.write(data[i]);
    //     Serial.print("write ");
    //     Serial.println(data[i]);
    // }
    
    int endPacketResult = mcp.endPacket();
    if (endPacketResult == 0)
    {
        Serial.println("Failed to end packet");
        return;
    }
    Serial.println("Packet sent");

}
