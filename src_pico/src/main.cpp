#include <Arduino.h>
#include <cybergear.h>
#include <can.h>

static void onReceiveExtendedCANPacket(int packetSize, long packetId, uint8_t *packetData)
{
    Serial.print("main.cpp: Received packet with id 0x");
    Serial.print(packetId, HEX); 
    cybergear_receive_can_message(packetSize, packetId, packetData);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) delay(10);
    Serial.println("Hello, we're live!");

    CanCommunication::init(onReceiveExtendedCANPacket);
    init_cybergear(&mi_motor[0], 126, Position_mode);
 
}

void loop()
{

    // Wait 3 seconds
    delay(3000);
    motor_position_control(&mi_motor[0], 0.2);
    delay(3000);
    motor_position_control(&mi_motor[0], 0.0);
  
}
