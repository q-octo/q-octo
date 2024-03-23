#include <Arduino.h>
#include "can.h"
#include "xiaomi_cybergear_driver.h"
#include "pico/stdlib.h"
#include <FreeRTOS.h> // Enables FreeRTOS and multicore support

// Interval:
#define TRANSMIT_RATE_MS 1000
#define POLLING_RATE_MS 1000

#define MOTOR_SPEED_LIMIT 10.0f
#define MOTOR_CURRENT_LIMIT 5.0f

unsigned long previousMillis = 0; // will store last time a message was send

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127
const uint8_t MASTER_CAN_ID = 0x00;
XiaomiCyberGearDriver cybergearL = XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_L, MASTER_CAN_ID);
XiaomiCyberGearDriver cybergearR = XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_R, MASTER_CAN_ID);

void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData, bool extended);
void initMotors();
void debugAlternateMotorSpeed();
void debugPrintMotorStatus();

void setup()
{
  // Start serial communication
  Serial.begin(0); // baud rate is ignored for USB serial
  while (!Serial)
    delay(10);
  Serial.println("Live on core 0");
  

  CanCommunication::init(onReceiveCanPacket);
  Serial.println("CAN init complete, setting up motor...");
  initMotors();
}

void loop()
{
  CanCommunication::checkForPacket();
  debugPrintMotorStatus();
  debugAlternateMotorSpeed();

  // send a request to the cybergear to receive motor status (position, speed, torque, temperature)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= TRANSMIT_RATE_MS)
  {
    previousMillis = currentMillis;
    cybergearL.request_status();
    cybergearR.request_status();
  }
}

// Setup code on Core 1
void setup1()
{
    Serial.begin(0); // baud rate is ignored for USB serial
  while (!Serial)
    delay(10);
  Serial.println("Live on core 1");
}

// Loop code on Core 1
void loop1()
{
}

void initMotors()
{
  cybergearL.init_motor(MODE_SPEED);
  cybergearL.set_limit_speed(MOTOR_SPEED_LIMIT);
  cybergearL.set_limit_current(MOTOR_CURRENT_LIMIT);
  cybergearL.enable_motor();
  cybergearR.init_motor(MODE_SPEED);
  cybergearR.set_limit_speed(MOTOR_SPEED_LIMIT);
  cybergearR.set_limit_current(MOTOR_CURRENT_LIMIT);
  cybergearR.enable_motor();
}

void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData, bool extendedb)
{
  Serial.print("main.cpp: Received packet with id 0x");
  Serial.print(packetId, HEX);

  switch ((packetId & 0xFF00) >> 8)
  {
  case CYBERGEAR_CAN_ID_L:
    cybergearL.process_message(packetData);
    break;
  case CYBERGEAR_CAN_ID_R:
    cybergearR.process_message(packetData);
    break;
  default:
    Serial.println("main.cpp: Received packet from unknown device");
    break;
  }
}

void debugAlternateMotorSpeed()
{
  cybergearR.set_speed_ref(0);
  cybergearL.set_speed_ref(2);
  delay(1000);
  cybergearL.set_speed_ref(0);
  cybergearR.set_speed_ref(2);
  delay(1000);
}

void debugPrintMotorStatus()
{
  XiaomiCyberGearStatus statusL = cybergearL.get_status();
  XiaomiCyberGearStatus statusR = cybergearR.get_status();
  Serial.printf("L: POS:%f V:%f T:%f temp:%d\n", statusL.position, statusL.speed, statusL.torque, statusL.temperature);
  Serial.printf("R: POS:%f V:%f T:%f temp:%d\n", statusR.position, statusR.speed, statusR.torque, statusR.temperature);
}