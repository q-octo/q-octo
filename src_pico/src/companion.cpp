#include <Arduino.h>
#include "companion.h"
#include "config.h"

namespace Companion {
  bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);

  SerialPIO companionSerial =
          SerialPIO(CFG_COMPANION_UART_TX, CFG_COMPANION_UART_RX, 32);
  // 1024 is the default size, but it will grow automatically.
  flatbuffers::FlatBufferBuilder fbb = flatbuffers::FlatBufferBuilder(1024);
  FlatbufferSerialParser fbSerialParser = FlatbufferSerialParser(companionSerial, verifyIncomingFlatbuffer);
}

void Companion::init() {
  if (!CFG_ENABLE_COMPANION) {
    return;
  }
  companionSerial.begin(115200);
}

void Companion::receiveMessage(const Message &message) {
  if (!CFG_ENABLE_COMPANION) {
    return;
  }
  switch (message.type) {
    case ENABLE_WEB_SERVER:
      Serial.println("Enable web server");
      break;
    case DISABLE_WEB_SERVER:
      Serial.println("Disable web server");
      break;
    case STATE_UPDATE:
      Serial.println("State update");
      DataManager::State state = message.as.state;
      sendStateToCompanion(state);
      break;
  }
}

void Companion::loop() {
  parseIncomingSerialData();
}

bool Companion::verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier) {
  return SizePrefixedCompanionTxBufferHasIdentifier(fbSerialParser.buffer) &&
         VerifySizePrefixedCompanionTxBuffer(verifier);
}

void Companion::parseIncomingSerialData() {
  if (!fbSerialParser.parseMessage()) {
    return;
  }
  auto companionMessage = GetSizePrefixedCompanionTx(fbSerialParser.buffer);
  handleCompanionTx(*companionMessage);
}

void Companion::handleCompanionTx(const CompanionTx &companionMessage) {
  switch (companionMessage.message_type()) {
    case CompanionTxUnion::CompanionTxUnion_NONE:
      break;
    case CompanionTxUnion::CompanionTxUnion_Update: {
      auto update = companionMessage.message_as_Update();
      handleUpdateMessage(*update);
      break;
    }
    case CompanionTxUnion::CompanionTxUnion_ButtonPressed: {
      auto buttonPressed = companionMessage.message_as_ButtonPressed();
      handleButtonPressedMessage(*buttonPressed);
      break;
    }
  }
}

void Companion::sendTaskMessage(const DataManager::Message &message) {
  DataManager::receiveMessage(message);
}

void Companion::handleUpdateMessage(const Update &update) {
  static DataManager::Message taskMessage;
  switch (update.update_type()) {
    case UpdateUnion::UpdateUnion_NONE:
      break;
    case UpdateUnion::UpdateUnion_UpdateBatteries: {
      auto updateBatteries = update.update_as_UpdateBatteries();
      taskMessage = {
              .type = DataManager::Type::SET_BATTERY_COUNT,
              .as = {.batteryCount = updateBatteries->batteries()},
      };
      sendTaskMessage(taskMessage);
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateLowVoltageThreshold: {
      auto updateLowVoltageThreshold =
              update.update_as_UpdateLowVoltageThreshold();
      taskMessage = {
              .type = DataManager::Type::SET_LOW_VOLTAGE_THRESHOLD,
              .as =
                      {
                              .voltageThreshold =
                              updateLowVoltageThreshold->low_voltage_threshold(),
                      },
      };
      sendTaskMessage(taskMessage);
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateCriticalVoltageThreshold: {
      auto updateCriticalVoltageThreshold =
              update.update_as_UpdateCriticalVoltageThreshold();
      taskMessage = {
              .type = DataManager::Type::SET_CRITICAL_VOLTAGE_THRESHOLD,
              .as =
                      {
                              .voltageThreshold = updateCriticalVoltageThreshold
                                      ->critical_voltage_threshold(),
                      },
      };
      sendTaskMessage(taskMessage);
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateReferenceWheelAngle: {
      auto updateReferenceWheelAngle =
              update.update_as_UpdateReferenceWheelAngle();
      // TODO implement
      // robot.reference_wheel_angle =
      // updateReferenceWheelAngle->reference_wheel_angle();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateFoldWheels: {
      // robot.wheels_folded = true;
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateRssiThreshold:
      break;
    case UpdateUnion::UpdateUnion_UpdateEnableRover:
      break;
    case UpdateUnion::UpdateUnion_UpdateLinkQualityThreshold:
      break;
  }
}

void Companion::handleButtonPressedMessage(const ButtonPressed &buttonPressed) {
  static DataManager::Message taskMessage;
  switch (buttonPressed.button()) {
    case Button_A:
      taskMessage = {
              .type = DataManager::Type::DISPLAY_BUTTON_PRESSED,
              .as = {.displayButton = DataManager::DisplayButton::A}};
      sendTaskMessage(taskMessage);
      break;
    case Button_B:
      taskMessage = {
              .type = DataManager::Type::DISPLAY_BUTTON_PRESSED,
              .as = {.displayButton = DataManager::DisplayButton::B}};
      sendTaskMessage(taskMessage);
      break;
    case Button_X:
      // Toggle wifi
      taskMessage = {.type = DataManager::Type::TOGGLE_WEB_SERVER_ENABLED};
      sendTaskMessage(taskMessage);
      break;
  }
}

void Companion::sendStateToCompanion(const DataManager::State &state) {
  Serial.println("Sending state to companion");
  serialiseState(state);
  companionSerial.write(fbb.GetBufferPointer(), fbb.GetSize());
}

void Companion::serialiseState(const DataManager::State &state) {
  fbb.Reset();

  RobotT robot = RobotT();
  stateToFlatbuffer(state, robot);

  auto robotOffset = CreateRobot(fbb, &robot);
  auto message = CreateCompanionRx(fbb, CompanionRxUnion_Robot, robotOffset.Union());
  FinishSizePrefixedCompanionRxBuffer(fbb, message);
}
