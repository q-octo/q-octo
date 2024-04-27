#include <Arduino.h>
#include <can.h>
#include "cybergear_rotary_encoder.h"
#include <xiaomi_cybergear_driver.h>

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127

const uint8_t TEST_MOTOR_CAN_ID = CYBERGEAR_CAN_ID_L;

CybergearRotaryEncoder encoder([](uint32_t packetId, uint8_t *packetData)
                               { CanCommunication::sendCANPacket(packetId, packetData, 8, false); },
                               0x01);

XiaomiCyberGearDriver motor = XiaomiCyberGearDriver(0x7E, 0x00);

void onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended);
void setup()
{
  Serial.begin(115200);
  // Wait for serial connection to be established
  while (!Serial)
    ;
  delay(1000); // Wait for a second
  Serial.println("Live on core 0 ofc");
  CanCommunication::init(onReceiveCanPacket);

  motor.stop_motor();
  delayMicroseconds(500);
  motor.set_run_mode(MODE_CURRENT);
  delayMicroseconds(500);
  motor.set_limit_speed(10);
  delayMicroseconds(500);
  motor.set_limit_current(4);
  delayMicroseconds(500);
  motor.enable_motor();
  delayMicroseconds(500);
  // motor.set_speed_ref(2);
  // motor.set_position_ref(6);

  // Serial.println("extended mode");
  // encoder.enableExtendedMode();
  // Serial.println("Sending start motor message");
  // encoder.startMotor();
  // delayMicroseconds(500);
  // // Serial.println("Setting motor position to zero");
  // // encoder.setMotorPositionToZero();
  // Serial.println("Enabling pst control mode");
  // encoder.positionSpeedTorqueTripleClosedLoopMode();
  // delayMicroseconds(500);
  // Serial.println("moving");
  // encoder.move(180, 10, 10, 10, 10);
  // delayMicroseconds(500);

  // XiaomiCyberGearMotionCommand cmd = {
  //     .position = 0,
  //     .speed = 1,
  //     .torque = 0,
  //     .kp = 1,
  //     .kd = 1};

  // motor.send_motion_control(cmd);
}

long lastTimeWeResetTarget = 0;

float target = 6;
int current = 0;

void loop()
{
  CanCommunication::checkForPacket();
  CanCommunication::checkForPacket();
  CanCommunication::checkForPacket();

  motor.set_current_ref(0.5);
  delay(100);
  auto status = motor.get_status();
  // Print position, speed, torque, temperature
  Serial.print("Position: ");
  Serial.println(status.position);
  Serial.print("Speed: ");
  Serial.println(status.speed);
  Serial.print("Torque: ");
  Serial.println(status.torque);
  Serial.print("Temperature: ");
  Serial.println(status.temperature);

  // if (millis() - lastTimeWeResetTarget > 5000)
  // {
  //   lastTimeWeResetTarget = millis();
  //   target = lastTimeWeResetTarget % 25;
  //   target -= 12.5;
  //   Serial.println("New target: ");
  //   Serial.println(target);

  //   XiaomiCyberGearMotionCommand cmd = {
  //       .position = target,
  //       .speed = 1,
  //       .torque = 0,
  //       .kp = 0,
  //       .kd = 0};

  //   motor.send_motion_control(cmd);
  // }

  // // Step towards target by 1 degree
  // if (current < target)
  // {
  //   current++;
  // }
  // else if (current > target)
  // {
  //   current--;
  // }

  // delay(1000);
  // Serial.println("Current position: ");
  // Serial.println(current);
}

void onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended)
{
  // Serial.print("Received packet with id 0x");
  // Serial.print(packetId, HEX);
  // Serial.println();

  // received a packet
  Serial.print("Received ");

  if (extended)
  {
    Serial.print("extended ");
  }
  else
  {
    Serial.print("standard ");
  }

  Serial.print("packet with id 0x");
  Serial.print(packetId, HEX);

  Serial.print(" and length ");
  Serial.println(packetLength);

  Serial.print("0x");
  for (size_t i = 0; i < packetLength; i++)
  {
    Serial.printf("%02X", packetData[i]);
  }
  Serial.println();

  if ((packetId & 0xFF00) >> 8 == 0x7E)
  {
    motor.process_message(packetId, packetData);
  }

  // if (packetId == 0x64)
  // {
  //   RotaryEncoderResponse response;
  //   encoder.processMessage(&response, packetLength, packetId, packetData);
  //   // Print position, speed, torque
  //   Serial.print("Position: ");
  //   Serial.println(response.position);
  //   Serial.print("Speed: ");
  //   Serial.println(response.speed);
  //   Serial.print("Torque: ");
  //   Serial.println(response.torque);
  // }
}
