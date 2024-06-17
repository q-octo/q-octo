/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <Arduino.h>
#include "computer.h"
#include "config.h"
#include "serial.h"

void Computer::init() {
  if (!CFG_ENABLE_ONBOARD_COMPUTER) {
    return;
  }

  computerSerial.setFIFOSize(1024);
  computerSerial.setPinout(CFG_ONBOARD_COMPUTER_UART_TX, CFG_ONBOARD_COMPUTER_UART_RX);
  computerSerial.begin(115200);
}

void Computer::receiveMessage(const Message &message) {
  if (!CFG_ENABLE_ONBOARD_COMPUTER) {
    return;
  }
  switch (message.type) {
    case STATE_UPDATE:
      sendStateToComputer(message.as.state);
      break;
    case CRSF_DATA:
      Serial.println("CRSF data");
      // TODO implement this
      break;
    case DISPLAY_BUTTON:
      sendDisplayButtonToComputer(message.as.displayButton);
      break;
  }
}

void Computer::loop() {
  parseIncomingSerialData();
}

bool Computer::verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier) {
  return SizePrefixedOnboardComputerTxBufferHasIdentifier(fbSerialParser.buffer) &&
         VerifySizePrefixedOnboardComputerTxBuffer(verifier);
}

void Computer::parseIncomingSerialData() {
  if (!fbSerialParser.parseMessage()) {
    return;
  }
  auto computerMessage = GetSizePrefixedOnboardComputerTx(fbSerialParser.buffer);
  handleComputerTx(*computerMessage);
}

void Computer::handleComputerTx(const fbs::OnboardComputerTx &computerMessage) {
  static DataManager::Message message;
  switch (computerMessage.message_type()) {
    case OnboardComputerTxUnion_NONE:
      break;
    case OnboardComputerTxUnion_CrsfFrame:
      // TODO implement if we can receive data on the transmitter.
      Serial.println("Received CRSF frame to forward to transmitter");
      break;
    case OnboardComputerTxUnion_DriveRobot: {
      Serial.println("Received drive robot message");
      auto driveMsg = computerMessage.message_as_DriveRobot()->UnPack();
      switch (driveMsg->config.type)
      {
        case DriveConfig_TankDriveConfig: {
          auto tankDriveConfig = driveMsg->config.AsTankDriveConfig();
          message.type = DataManager::Type::SET_MOTOR_SPEED_INDIVIDUAL;
          message.as.motorSpeedIndividual = {
            .leftSpeed = tankDriveConfig->left_rad_s,
            .rightSpeed = tankDriveConfig->right_rad_s,
            .controlSource = DataManager::RobotControlSource::ONBOARD_COMPUTER,
          };
          DataManager::receiveMessage(message);
          break;
        }
        case DriveConfig_MixedDriveConfig: {
          auto mixedDriveConfig = driveMsg->config.AsMixedDriveConfig();
          message.type = DataManager::Type::SET_MOTOR_SPEED_COMBINED;
          message.as.motorSpeedCombined = {
            .speed = mixedDriveConfig->rad_s,
            .direction = mixedDriveConfig->direction,
            .controlSource = DataManager::RobotControlSource::ONBOARD_COMPUTER,
          };
          DataManager::receiveMessage(message);
          break;
        }
        default:
          break;
      }
      break;
    }
    case OnboardComputerTxUnion_DisplayMessages: {
      Serial.println("Received display messages");
      message.type = DataManager::Type::DISPLAY_MESSAGES;
      auto messages = computerMessage.message_as_DisplayMessages();

      static std::string message1 = messages->message1()->str();
      message.as.displayMessages.message1 = &message1;
      static std::string message2 = messages->message2()->str();
      message.as.displayMessages.message2 = &message2;
      static std::string message3 = messages->message3()->str();
      message.as.displayMessages.message3 = &message3;
      static std::string message4 = messages->message4()->str();
      message.as.displayMessages.message4 = &message4;
      static std::string message5 = messages->message5()->str();
      message.as.displayMessages.message5 = &message5;
      static std::string message6 = messages->message6()->str();
      message.as.displayMessages.message6 = &message6;
      static std::string message7 = messages->message7()->str();
      message.as.displayMessages.message7 = &message7;

      DataManager::receiveMessage(message);
      break;
    }
  }
}


void Computer::sendStateToComputer(const DataManager::State &state) {
  Serial.println("Sending state to onboard computer");
  serialiseState(state);
  static SendSerialData serialData = {
          .target = ONBOARD_COMPUTER,
          .data = fbb.GetBufferPointer(),
          .size = fbb.GetSize()
  };
  xQueueSend(SerialTask::queue, &serialData, 0);
}

Button Computer::serialiseDisplayButton(const DataManager::DisplayButton &displayButton) {
  switch (displayButton) {
    case DataManager::DisplayButton::A:
      return Button_A;
    case DataManager::DisplayButton::B:
      return Button_B;
  }
  return Button_A;
}

void Computer::sendDisplayButtonToComputer(const DataManager::DisplayButton &displayButton) {
  Serial.println("Sending display button to onboard computer");
  fbb.Reset();

  auto displayButtonOffset = CreateButtonPressed(fbb, serialiseDisplayButton(displayButton));
  auto message = CreateOnboardComputerRx(fbb, OnboardComputerRxUnion_ButtonPressed, displayButtonOffset.Union());
  FinishSizePrefixedOnboardComputerRxBuffer(fbb, message);
  static SendSerialData serialData = {
          .target = ONBOARD_COMPUTER,
          .data = fbb.GetBufferPointer(),
          .size = fbb.GetSize()
  };
  xQueueSend(SerialTask::queue, &serialData, 0);
}

void Computer::serialiseState(const DataManager::State &state) {
  fbb.Reset();

  RobotT robot = RobotT();
  stateToFlatbuffer(state, robot);

  auto robotOffset = CreateRobot(fbb, &robot);
  auto message = CreateOnboardComputerRx(fbb, OnboardComputerRxUnion_Robot, robotOffset.Union());
  FinishSizePrefixedOnboardComputerRxBuffer(fbb, message);
}
