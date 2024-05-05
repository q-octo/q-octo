#include <Arduino.h>
#include "companion.h"
#include "config.h"
#include "storage.h"

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
  Storage::State &state = Storage::getState();
  switch (update.update_type()) {
    case UpdateUnion::UpdateUnion_NONE:
      break;
    case UpdateUnion::UpdateUnion_UpdateBatteries: {
      auto updateBatteries = update.update_as_UpdateBatteries();
      state.batteryCount = updateBatteries->batteries();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateLowVoltageThreshold: {
      auto updateLowVoltageThreshold =
              update.update_as_UpdateLowVoltageThreshold();
      state.lowVoltageThreshold = updateLowVoltageThreshold->low_voltage_threshold();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateCriticalVoltageThreshold: {

      auto updateCriticalVoltageThreshold =
              update.update_as_UpdateCriticalVoltageThreshold();
      state.criticalVoltageThreshold = updateCriticalVoltageThreshold->critical_voltage_threshold();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateReferenceWheelAngle: {
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
      state.rssiThreshold = update.update_as_UpdateRssiThreshold()->rssi_threshold();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateLinkQualityThreshold: {
      state.linkQualityThreshold = update.update_as_UpdateLinkQualityThreshold()->link_quality_threshold();
      Storage::save();
      break;
    }
    case UpdateUnion::UpdateUnion_UpdateEnableRover:
      // TODO implement rover enable/disable
      break;
    case UpdateUnion::UpdateUnion_UpdateStartWebServerOnLaunch:
      // TODO we still need to check this flag on launch
      state.startWebServerOnLaunch = update.update_as_UpdateStartWebServerOnLaunch()->start_web_server_on_launch();
      Storage::save();
      break;
    case UpdateUnion_UpdateMaxSpeed: {
      taskMessage = {
              .type = DataManager::Type::UPDATE_MOTOR_SPEED_LIMIT,
              .as = {.motorLimit = update.update_as_UpdateMaxSpeed()->max_speed()}};
      DataManager::receiveMessage(taskMessage);
      // state.motorSpeedLimit = update.update_as_UpdateMaxSpeed()->max_speed();
      // Storage::save();
      break;
    }
    case UpdateUnion_UpdateMaxCurrent: {
      taskMessage = {
              .type = DataManager::Type::UPDATE_MOTOR_CURRENT_LIMIT,
              .as = {.motorLimit = update.update_as_UpdateMaxCurrent()->max_current()}};
      DataManager::receiveMessage(taskMessage);
      // state.motorCurrentLimit = update.update_as_UpdateMaxCurrent()->max_current();
      // Storage::save();
      break;
    }
    case UpdateUnion_UpdateMaxTorque: {
      taskMessage = {
              .type = DataManager::Type::UPDATE_MOTOR_TORQUE_LIMIT,
              .as = {.motorLimit = update.update_as_UpdateMaxTorque()->max_torque()}};
      DataManager::receiveMessage(taskMessage);
      // state.motorTorqueLimit = update.update_as_UpdateMaxTorque()->max_torque();
      // Storage::save();
      break;
    }
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

  uint32_t currentMillis = millis();
  if (currentMillis - msSinceLastBroadcast < 500) {
    Serial.println("Not sending state to companion, too soon");
    Serial.println(currentMillis - msSinceLastBroadcast);
    return;
  }
  msSinceLastBroadcast = currentMillis;
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
