#include <Arduino.h>
#include "companion.h"
#include "config.h"
#include "storage.h"

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
      Storage::State &state = Storage::getState();
      state.batteryCount = updateBatteries->batteries();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateLowVoltageThreshold: {
      auto updateLowVoltageThreshold =
              update.update_as_UpdateLowVoltageThreshold();

      Storage::State &state = Storage::getState();
      state.lowVoltageThreshold = updateLowVoltageThreshold->low_voltage_threshold();
      Storage::save(); 
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateCriticalVoltageThreshold: {

      auto updateCriticalVoltageThreshold =
              update.update_as_UpdateCriticalVoltageThreshold();
      Storage::State &state = Storage::getState();
      state.criticalVoltageThreshold = updateCriticalVoltageThreshold->critical_voltage_threshold();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateReferenceWheelAngle: {
      Storage::State &state = Storage::getState();
      state.leftMotorFoldAngle = update.update_as_UpdateReferenceWheelAngle()->left_motor_fold_angle();
      state.rightMotorFoldAngle = update.update_as_UpdateReferenceWheelAngle()->right_motor_fold_angle();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateFoldWheels: {
      // TODO implement wheel folding
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateRssiThreshold: {
      Storage::State &state = Storage::getState();
      state.rssiThreshold = update.update_as_UpdateRssiThreshold()->rssi_threshold();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateLinkQualityThreshold: {
      Storage::State &state = Storage::getState();
      state.linkQualityThreshold = update.update_as_UpdateLinkQualityThreshold()->link_quality_threshold();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateEnableRover:
      // TODO implement rover enable/disable
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
