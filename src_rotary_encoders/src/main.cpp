#include <Arduino.h>
#include <can.h>
#include "cybergear_rotary_encoder.h"
#include <xiaomi_cybergear_driver.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <LEAmDNS.h>
#include <flatbuffers/flatbuffers.h>
#include "debug_rotary_encoder_generated.h"

const uint8_t CYBERGEAR_CAN_ID_L = 0x7E; // 126
const uint8_t CYBERGEAR_CAN_ID_R = 0x7F; // 127

const uint8_t TEST_MOTOR_CAN_ID = CYBERGEAR_CAN_ID_L;

CybergearRotaryEncoder encoder(
        [](uint32_t packetId, uint8_t *packetData) { CanCommunication::sendCANPacket(packetId, packetData, 8, false); },
        0x01);

CybergearRotaryEncoder encoderL(
        [](uint32_t packetId, uint8_t *packetData) {
          CanCommunication::sendCANPacket(packetId, packetData, 8, false);
        },
        0x01);

CybergearRotaryEncoder encoderR(
        [](uint32_t packetId, uint8_t *packetData) {
          CanCommunication::sendCANPacket(packetId, packetData, 8, false);
        },
        0x02);

XiaomiCyberGearDriver motor = XiaomiCyberGearDriver(0x7E, 0x00);
XiaomiCyberGearDriver motorL = XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_L, 0x00);
XiaomiCyberGearDriver motorR = XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_R, 0x00);

IPAddress apIP;

WebSocketsServer *webSocket = nullptr;
flatbuffers::FlatBufferBuilder fbb;

void onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

void setup() {
  Serial.begin(115200);
  // Wait for serial connection to be established
  while (!Serial);
  delay(1000); // Wait for a second
  Serial.println("Live on core 0 ofc");
  CanCommunication::init(onReceiveCanPacket);

  // Web socket server
  Serial.println("Starting wifi");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Q-Octo (rover.local)");
  delayMicroseconds(500 * 1000); // Without delay the IP might be blank (according to an example)
  Serial.print("AP IP address: ");
  Serial.printf("%s (rover.local)\n", WiFi.softAPIP().toString().c_str());

  webSocket = new WebSocketsServer(81);
  webSocket->onEvent(webSocketEvent);

  if (!MDNS.begin("rover")) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
    // Add service to mDNS
//    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
  }

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

void loop() {
  webSocket->loop();
  MDNS.update();
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



void printCommand(MotorControl::CommandT &command) {
  using namespace MotorControl;

  Serial.print("Command Type: ");
  switch (command.type) {
    case CommandType_MOTOR_COMMAND:
      Serial.println("Motor Command");
      if (command.motor_control != nullptr) {
        MotorCommandT motorCommand = *command.motor_control;
        Serial.print("Motor ID: ");
        Serial.println(motorCommand.motor_id);
        switch (motorCommand.command.type) {
          case MotorCommands_InitMotor:
            Serial.println("Init Motor");
            Serial.print("Mode: ");
            Serial.println(motorCommand.command.AsInitMotor()->mode);
            break;
          case MotorCommands_EnableMotor:
            Serial.println("Enable Motor");
            break;
          case MotorCommands_StopMotor:
            Serial.println("Stop Motor");
            break;
          case MotorCommands_SetRunMode:
            Serial.println("Set Run Mode");
            Serial.print("Mode: ");
            Serial.println(motorCommand.command.AsSetRunMode()->mode);
            break;
          case MotorCommands_SendMotionControl:
            Serial.println("Send Motion Control");
            // Additional fields can be printed similarly
            break;
          case MotorCommands_SetLimitSpeed:
            Serial.println("Set Limit Speed");
            Serial.print("Speed: ");
            Serial.println(motorCommand.command.AsSetLimitSpeed()->speed);
            break;
          case MotorCommands_SetLimitCurrent:
            Serial.println("Set Limit Current");
            Serial.print("Current: ");
            Serial.println(motorCommand.command.AsSetLimitCurrent()->current);
            break;
          case MotorCommands_SetLimitTorque:
            Serial.println("Set Limit Torque");
            Serial.print("Torque: ");
            Serial.println(motorCommand.command.AsSetLimitTorque()->torque);
            break;
          default:
            Serial.println("Other Motor Command");
            break;
        }
      }
      break;
    case CommandType_ENCODER_COMMAND:
      Serial.println("Encoder Command");
      if (command.encoder_control != nullptr) {
        EncoderCommandT encoderCommand = *command.encoder_control;
        Serial.print("Encoder ID: ");
        Serial.println(encoderCommand.motor_id);
        switch (encoderCommand.command.type) {
          case EncoderCommands_EnableExtendedMode:
            Serial.println("Enable Extended Mode");
            break;
          case EncoderCommands_StartMotor:
            Serial.println("Start Motor");
            break;
          case EncoderCommands_StopMotor:
            Serial.println("Stop Motor");
            break;
          case EncoderCommands_TorqueControlMode:
            Serial.println("Torque Control Mode");
            break;
          case EncoderCommands_SpeedTorqueControlMode:
            Serial.println("Speed Torque Control Mode");
            break;
          case EncoderCommands_PositionSpeedTorqueMode:
            Serial.println("Position Speed Torque Mode");
            break;
          case EncoderCommands_SetMotorPositionToZero:
            Serial.println("Set Motor Position To Zero");
            break;
          case EncoderCommands_MoveMotor:
            Serial.println("Move Motor");
            // Additional fields can be printed similarly
            break;
          default:
            Serial.println("Other Encoder Command");
            break;
        }
      }
      break;
    default:
      Serial.println("Unknown Command Type");
      break;
  }
}


void parseFlatbufferMessage(uint8_t *payload) {
  using namespace MotorControl;
  CommandT command;
  GetCommand(payload)->UnPackTo(&command);
  printCommand(command);


  if (command.type == CommandType_MOTOR_COMMAND) {
    MotorCommandT motorCommand = *command.motor_control;
    const int8_t motorId = motorCommand.motor_id;
    switch (motorCommand.command.type) {
      case MotorCommands_NONE:
        break;
      case MotorCommands_InitMotor: {
        InitMotorT initMotor = *motorCommand.command.AsInitMotor();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.init_motor(initMotor.mode);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.init_motor(initMotor.mode);
        }
        break;
      }
      case MotorCommands_EnableMotor:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.enable_motor();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.enable_motor();
        }
        break;
      case MotorCommands_StopMotor:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.stop_motor();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.stop_motor();
        }
        break;
      case MotorCommands_SetRunMode: {
        SetRunModeT setRunMode = *motorCommand.command.AsSetRunMode();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_run_mode(setRunMode.mode);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_run_mode(setRunMode.mode);
        }
        break;
      }
      case MotorCommands_SendMotionControl: {
        SendMotionControlT sendMotionControl = *motorCommand.command.AsSendMotionControl();
        XiaomiCyberGearMotionCommand cmd = {
                .position = sendMotionControl.position,
                .speed = sendMotionControl.speed,
                .torque = sendMotionControl.torque,
                .kp = sendMotionControl.kp,
                .kd = sendMotionControl.kd};
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.send_motion_control(cmd);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.send_motion_control(cmd);
        }
        break;
      }
      case MotorCommands_SetLimitSpeed: {
        SetLimitSpeedT setLimitSpeed = *motorCommand.command.AsSetLimitSpeed();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_limit_speed(setLimitSpeed.speed);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_limit_speed(setLimitSpeed.speed);
        }
        break;
      }
      case MotorCommands_SetLimitCurrent: {
        SetLimitCurrentT setLimitCurrent = *motorCommand.command.AsSetLimitCurrent();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_limit_current(setLimitCurrent.current);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_limit_current(setLimitCurrent.current);
        }
        break;
      }
      case MotorCommands_SetLimitTorque: {
        SetLimitTorqueT setLimitTorque = *motorCommand.command.AsSetLimitTorque();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_limit_torque(setLimitTorque.torque);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_limit_torque(setLimitTorque.torque);
        }
        break;
      }
      case MotorCommands_SetCurrentKp: {
        SetCurrentKpT setCurrentKp = *motorCommand.command.AsSetCurrentKp();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_current_kp(setCurrentKp.kp);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_current_kp(setCurrentKp.kp);
        }
        break;
      }
      case MotorCommands_SetCurrentKi: {
        SetCurrentKiT setCurrentKi = *motorCommand.command.AsSetCurrentKi();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_current_ki(setCurrentKi.ki);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_current_ki(setCurrentKi.ki);
        }
        break;
      }
      case MotorCommands_SetCurrentFilterGain: {
        SetCurrentFilterGainT setCurrentFilterGain = *motorCommand.command.AsSetCurrentFilterGain();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_current_filter_gain(setCurrentFilterGain.gain);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_current_filter_gain(setCurrentFilterGain.gain);
        }
        break;
      }
      case MotorCommands_SetCurrentRef: {
        SetCurrentRefT setCurrentRef = *motorCommand.command.AsSetCurrentRef();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_current_ref(setCurrentRef.current_ref);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_current_ref(setCurrentRef.current_ref);
        }
        break;
      }
      case MotorCommands_SetPositionKp: {
        SetPositionKpT setPositionKp = *motorCommand.command.AsSetPositionKp();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_position_kp(setPositionKp.kp);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_position_kp(setPositionKp.kp);
        }
        break;
      }
      case MotorCommands_SetPositionRef: {
        SetPositionRefT setPositionRef = *motorCommand.command.AsSetPositionRef();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_position_ref(setPositionRef.position_ref);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_position_ref(setPositionRef.position_ref);
        }
        break;
      }
      case MotorCommands_SetSpeedKp: {
        SetSpeedKpT setSpeedKp = *motorCommand.command.AsSetSpeedKp();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_speed_kp(setSpeedKp.kp);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_speed_kp(setSpeedKp.kp);
        }
        break;
      }
      case MotorCommands_SetSpeedKi: {
        SetSpeedKiT setSpeedKi = *motorCommand.command.AsSetSpeedKi();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_speed_ki(setSpeedKi.ki);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_speed_ki(setSpeedKi.ki);
        }
        break;
      }
      case MotorCommands_SetSpeedRef: {
        SetSpeedRefT setSpeedRef = *motorCommand.command.AsSetSpeedRef();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          motorL.set_speed_ref(setSpeedRef.speed_ref);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          motorR.set_speed_ref(setSpeedRef.speed_ref);
        }
        break;
      }
    }
  }

  if (command.type == CommandType_ENCODER_COMMAND) {
    EncoderCommandT encoderCommand = *command.encoder_control;
    const int8_t motorId = encoderCommand.motor_id;
    switch (encoderCommand.command.type) {
      case MotorControl::EncoderCommands_NONE:
        break;
      case EncoderCommands_EnableExtendedMode:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          encoderL.enableExtendedMode();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          encoderR.enableExtendedMode();
        }
        break;
      case EncoderCommands_StartMotor:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          encoderL.startMotor();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          encoderR.startMotor();
        }
        break;
      case EncoderCommands_StopMotor:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          encoderL.stopMotor();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          encoderR.stopMotor();
        }
        break;
      case EncoderCommands_TorqueControlMode:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          encoderL.torqueControlMode();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          encoderR.torqueControlMode();
        }
        break;
      case EncoderCommands_SpeedTorqueControlMode:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          encoderL.speedTorqueControlMode();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          encoderR.speedTorqueControlMode();
        }
        break;
      case EncoderCommands_PositionSpeedTorqueMode:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          encoderL.positionSpeedTorqueTripleClosedLoopMode();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          encoderR.positionSpeedTorqueTripleClosedLoopMode();
        }
        break;
      case EncoderCommands_SetMotorPositionToZero:
        if (motorId == CYBERGEAR_CAN_ID_L) {
          encoderL.setMotorPositionToZero();
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          encoderR.setMotorPositionToZero();
        }
        break;
      case EncoderCommands_MoveMotor: {
        MoveMotorT moveMotor = *encoderCommand.command.AsMoveMotor();
        if (motorId == CYBERGEAR_CAN_ID_L) {
          encoderL.move(moveMotor.position_degrees, moveMotor.speed, moveMotor.position_gain,
                        moveMotor.position_damping, moveMotor.torque);
        } else if (motorId == CYBERGEAR_CAN_ID_R) {
          encoderR.move(moveMotor.position_degrees, moveMotor.speed, moveMotor.position_gain,
                        moveMotor.position_damping, moveMotor.torque);
        }
        break;
      }
    }
  }
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: {
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    }
    case WStype_CONNECTED: {
      IPAddress ip = webSocket->remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      break;
    }
    case WStype_TEXT: {
      Serial.printf("[%u] get Text: %s\n", num, payload);
      break;
    }
    case WStype_BIN: {
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      parseFlatbufferMessage(payload);
      break;
    }
    default:
      Serial.println("[WARN] Unexpected websocket event");
      break;
  }
}

void onReceiveCanPacket(uint8_t packetLength, uint32_t packetId, uint8_t *packetData,
                        bool extended) {
  // Serial.print("Received packet with id 0x");
  // Serial.print(packetId, HEX);
  // Serial.println();

  // received a packet
  Serial.print("Received ");

  if (extended) {
    Serial.print("extended ");
  } else {
    Serial.print("standard ");
  }

  Serial.print("packet with id 0x");
  Serial.print(packetId, HEX);

  Serial.print(" and length ");
  Serial.println(packetLength);

  Serial.print("0x");
  for (size_t i = 0; i < packetLength; i++) {
    Serial.printf("%02X", packetData[i]);
  }
  Serial.println();

  if ((packetId & 0xFF00) >> 8 == 0x7E) {
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
