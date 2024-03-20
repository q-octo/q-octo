#include <Arduino.h>
// #include "cybergear.h"
#include "can.h"
#include "xiaomi_cybergear_driver.h"

// Interval:
#define TRANSMIT_RATE_MS 1000
#define POLLING_RATE_MS 1000

unsigned long previousMillis = 0; // will store last time a message was send

uint8_t CYBERGEAR_CAN_ID_L = 0x7E;
uint8_t CYBERGEAR_CAN_ID_R = 0x7F;
uint8_t MASTER_CAN_ID = 0x00;
XiaomiCyberGearDriver cybergearL = XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_L, MASTER_CAN_ID);
XiaomiCyberGearDriver cybergearR = XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_R, MASTER_CAN_ID);

static void onReceiveExtendedCANPacket(int packetSize, uint32_t packetId, uint8_t *packetData)
{
  Serial.print("main.cpp: Received packet with id 0x");
  Serial.print(packetId, HEX);

  if (((packetId & 0xFF00) >> 8) == CYBERGEAR_CAN_ID_L)
  {
    cybergearL.process_message(packetData);
  }

  // cybergear_receive_can_message(packetSize, packetId, packetData);
  // process_message(packetData);
}



/*
static void setupViaLibrary()
{
  cybergearL.init_motor(MODE_POSITION);
  Serial.println("init_motor");
  cybergear.set_limit_speed(10.0f); // set the maximum speed of the motor
  Serial.println("set_limit_speed");
  cybergear.set_limit_current(5.0); // current limit allows faster operation 
  Serial.println("set_limit_current");
  cybergear.enable_motor();        // turn on the motor 
  Serial.println("enable_motor");

  cybergear.set_position_ref(0.0); // set initial rotor position
  Serial.println("set_position_ref");
  cybergear.stop_motor();          // stop the motor
  Serial.println("stop_motor");
}
*/

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  Serial.println("Hello, we're live!");

  CanCommunication::init(onReceiveExtendedCANPacket);
  // delay(500);
  Serial.println("CAN init complete, setting up motor...");
  cybergearL.init_motor(MODE_SPEED);
  cybergearL.set_limit_speed(10.0f);    /* set the maximum speed of the motor */
  cybergearL.set_limit_current(5.0);    /* current limit allows faster operation */
  cybergearL.enable_motor();            /* turn on the motor */
  // cybergear.set_position_ref(0.0);     /* set initial rotor position */
  cybergearR.init_motor(MODE_SPEED);
  cybergearR.set_limit_speed(10.0f);    /* set the maximum speed of the motor */
  cybergearR.set_limit_current(5.0);    /* current limit allows faster operation */
  cybergearR.enable_motor();            /* turn on the motor */

  

  // delay(500);
  // cybergear.request_status();
  // Serial.println("Requested status");
}

void loop()
{ 
  delay(1000);
  XiaomiCyberGearStatus cybergear_status = cybergearL.get_status();
  char buffer[100]; // Adjust the size as necessary
  sprintf(buffer, "POS:%f V:%f T:%f temp:%d\n", cybergear_status.position, cybergear_status.speed, cybergear_status.torque, cybergear_status.temperature);
  Serial.print(buffer);

  cybergearR.set_speed_ref(0);
  cybergearL.set_speed_ref(2);
  delay(1000);
  cybergearL.set_speed_ref(0);
  cybergearR.set_speed_ref(2);

    // cybergear.set_speed_ref(12);

  // cybergear.set_position_ref(1);
  // delay(1000);
  // cybergear.set_position_ref(-1);

    // send a request to the cybergear to receive motor status (position, speed, torque, temperature)
  // unsigned long currentMillis = millis();
  // if (currentMillis - previousMillis >= TRANSMIT_RATE_MS) {
  //   previousMillis = currentMillis;
  //   cybergearL.request_status();
  // }
}