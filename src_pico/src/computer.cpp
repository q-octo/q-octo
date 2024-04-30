#include <Arduino.h>
#include "computer.h"
#include "config.h"

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

void Computer::handleComputerTx(const OnboardComputerTx &computerMessage) {
  switch (computerMessage.message_type()) {
    case OnboardComputerTxUnion_NONE:
      break;
    case OnboardComputerTxUnion_CrsfFrame:
      // TODO implement if we can receive data on the transmitter.
      Serial.println("Received CRSF frame to forward to transmitter");
      break;
    case OnboardComputerTxUnion_DriveRobot:
      // TODO implement driving via the onboard computer
      // Fun...
      Serial.println("Received drive robot message");
      break;
    case OnboardComputerTxUnion_DisplayMessages: {
      Serial.println("Received display messages");
      DataManager::Message message;
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

      sendTaskMessage(message);
      break;
    }
  }
}

void Computer::sendTaskMessage(const DataManager::Message &message) {
  DataManager::receiveMessage(message);
}

void Computer::sendStateToComputer(const DataManager::State &state) {
  Serial.println("Sending state to onboard computer");
  serialiseState(state);
  computerSerial.write(fbb.GetBufferPointer(), fbb.GetSize());
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
  computerSerial.write(fbb.GetBufferPointer(), fbb.GetSize());
}

void Computer::serialiseState(const DataManager::State &state) {
  fbb.Reset();

  RobotT robot = RobotT();
  stateToFlatbuffer(state, robot);

  auto robotOffset = CreateRobot(fbb, &robot);
  auto message = CreateOnboardComputerRx(fbb, OnboardComputerRxUnion_Robot, robotOffset.Union());
  FinishSizePrefixedOnboardComputerRxBuffer(fbb, message);
}
