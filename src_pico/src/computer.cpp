#include <Arduino.h>
#include "computer.h"
#include "config.h"

void Computer::receiveMessage(const Message &message) {
#if !CFG_ENABLE_ONBOARD_COMPUTER
  return;
#endif
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
    // TODO handle these cases
    case OnboardComputerTxUnion_NONE:
      break;
    case OnboardComputerTxUnion_CrsfFrame:
      Serial.println("Received CRSF frame to forward to transmitter");
      break;
    case OnboardComputerTxUnion_DriveRobot:
      Serial.println("Received drive robot message");
      break;
  }

}

void Computer::sendTaskMessage(const TaskMessage::Message &message) {
  xQueueSend(dataManagerQueue, &message, 0);
}

void Computer::task(void *pvParameters) {
  (void) pvParameters; //  To avoid warnings
  Serial.println("Started onboard computer producer task");

  computerSerial.setPinout(CFG_ONBOARD_COMPUTER_UART_TX, CFG_ONBOARD_COMPUTER_UART_RX);
  computerSerial.begin(115200);

  for (;;) {
    loop();
    vTaskDelay(pdMS_TO_TICKS(8));
  }
}


void Computer::sendStateToComputer(const TaskMessage::State &state) {
  Serial.println("Sending state to onboard computer");
  serialiseState(state);
  computerSerial.write(fbb.GetBufferPointer(), fbb.GetSize());
}

//void Computer::sendCrsfDataToComputer(const TaskMessage:: &crsfFrame) {
//  fbb.Reset();
//
//  auto crsfFrameOffset = CreateCrsfFrame(fbb, &crsfFrame);
//  auto message = CreateOnboardComputerRx(fbb, OnboardComputerRxUnion_CrsfData, crsfFrameOffset.Union());
//  FinishSizePrefixedOnboardComputerRxBuffer(fbb, message);
//  computerSerial.write(fbb.GetBufferPointer(), fbb.GetSize());
//}

Button Computer::serialiseDisplayButton(const TaskMessage::DisplayButton &displayButton) {
  switch (displayButton) {
    case TaskMessage::DisplayButton::A:
      return Button_A;
    case TaskMessage::DisplayButton::B:
      return Button_B;
  }
  return Button_A;
}

void Computer::sendDisplayButtonToComputer(const TaskMessage::DisplayButton &displayButton) {
  Serial.println("Sending display button to onboard computer");
  fbb.Reset();

  auto displayButtonOffset = CreateButtonPressed(fbb, serialiseDisplayButton(displayButton));
  auto message = CreateOnboardComputerRx(fbb, OnboardComputerRxUnion_ButtonPressed, displayButtonOffset.Union());
  FinishSizePrefixedOnboardComputerRxBuffer(fbb, message);
  computerSerial.write(fbb.GetBufferPointer(), fbb.GetSize());
}

void Computer::serialiseState(const TaskMessage::State &state) {
  fbb.Reset();

  RobotT robot = RobotT();
  stateToFlatbuffer(state, robot);

  auto robotOffset = CreateRobot(fbb, &robot);
  auto message = CreateOnboardComputerRx(fbb, OnboardComputerRxUnion_Robot, robotOffset.Union());
  FinishSizePrefixedOnboardComputerRxBuffer(fbb, message);
}
