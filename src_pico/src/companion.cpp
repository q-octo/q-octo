#include <Arduino.h>
#include "companion.h"
#include "config.h"
#include "storage.h"
#include "serial.h"

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
    case STATE_UPDATE:
      Serial.println("State update");
      pendingBroadcast = true;
      break;
  }
}

void Companion::loop() {
  if (!CFG_ENABLE_COMPANION) {
    return;
  }
  parseIncomingSerialData();

  const uint32_t currentMillis = millis();
  
  if (pendingBroadcast && currentMillis - msSinceLastBroadcast > 500) {
    sendStateToCompanion(DataManager::getState());
  }
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
  Serial.println("GOT COMPANION MESSAGE");
  switch (companionMessage.message_type()) {
    case CompanionTxUnion::CompanionTxUnion_NONE:
      break;
    case CompanionTxUnion::CompanionTxUnion_Update: {
      Serial.println("GOT UPDATE MESSAGE");
      auto update = companionMessage.message_as_Update();
      handleUpdateMessage(*update);
      break;
    }
    case CompanionTxUnion::CompanionTxUnion_ButtonPressed: {
      Serial.println("GOT BUTTON PRESSED MESSAGE");
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
  StorageState &state = Storage::getState();
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
      state.startWebServerOnLaunch = update.update_as_UpdateStartWebServerOnLaunch()->start_web_server_on_launch();
      Storage::save();
      break;
    case UpdateUnion_UpdateMaxSpeed: {
      taskMessage = {
              .type = DataManager::Type::UPDATE_MOTOR_SPEED_LIMIT,
              .as = {.floatMotorParam = update.update_as_UpdateMaxSpeed()->max_speed()}};
      DataManager::receiveMessage(taskMessage);
      state.motorSpeedLimit = update.update_as_UpdateMaxSpeed()->max_speed();
      Storage::save();
      break;
    }
    case UpdateUnion_UpdateMaxCurrent: {
      taskMessage = {
              .type = DataManager::Type::UPDATE_MOTOR_CURRENT_LIMIT,
              .as = {.floatMotorParam = update.update_as_UpdateMaxCurrent()->max_current()}};
      DataManager::receiveMessage(taskMessage);
      state.motorCurrentLimit = update.update_as_UpdateMaxCurrent()->max_current();
      Storage::save();
      break;
    }
    case UpdateUnion_UpdateMaxTorque: {
      taskMessage = {
              .type = DataManager::Type::UPDATE_MOTOR_TORQUE_LIMIT,
              .as = {.floatMotorParam = update.update_as_UpdateMaxTorque()->max_torque()}};
      DataManager::receiveMessage(taskMessage);
      state.motorTorqueLimit = update.update_as_UpdateMaxTorque()->max_torque();
      Storage::save();
      break;
    }
    case UpdateUnion_UpdateMotorKp: {
      taskMessage = {
              .type = DataManager::Type::UPDATE_MOTOR_SPEED_KP,
              .as = {.floatMotorParam = update.update_as_UpdateMotorKp()->motor_kp()}};
      DataManager::receiveMessage(taskMessage);
      state.speedKp = update.update_as_UpdateMotorKp()->motor_kp();
      Storage::save();
      break;
    }
    case UpdateUnion_UpdateMotorKi: {
      taskMessage = {
              .type = DataManager::Type::UPDATE_MOTOR_SPEED_KI,
              .as = {.floatMotorParam = update.update_as_UpdateMotorKi()->motor_ki()}};
      DataManager::receiveMessage(taskMessage);
      state.speedKi = update.update_as_UpdateMotorKi()->motor_ki();
      Storage::save();
      break;
    }
    case UpdateUnion_UpdateCrsfLinkStatsTimeout: {
      state.radioReceiverTimeoutMillis = update.update_as_UpdateCrsfLinkStatsTimeout()->timeout_millis();
      Storage::save();
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
      taskMessage = {.type = DataManager::Type::DISPLAY_WEB_SERVER_BTN_PRESSED};
      sendTaskMessage(taskMessage);
      break;
  }
}

void Companion::sendStateToCompanion(const DataManager::State &state) {
  uint32_t currentMillis = millis();
  msSinceLastBroadcast = currentMillis;
  pendingBroadcast = false;
  Serial.printf("Sending state to companion: %d\n", fbb.GetSize());
  serialiseState(state);
  static SendSerialData data = {
          .target = COMPANION,
          .data = fbb.GetBufferPointer(),
          .size = fbb.GetSize()
  };
  companionSerial.write(FlatbufferSerialParser::START_BYTE);
  if (!companionSerial.write(fbb.GetBufferPointer(), fbb.GetSize())) {
    Serial.println("FAILED to send to companion");
  }
  // Serial.printf("Took %d ms to send to companion\n", millis() - currentMillis);
  // xQueueSend(SerialTask::queue, &data, 0);
}

void Companion::serialiseState(const DataManager::State &state) {
  fbb.Reset();

  RobotT robot = RobotT();
  stateToFlatbuffer(state, robot);

  auto robotOffset = CreateRobot(fbb, &robot);
  auto message = CreateCompanionRx(fbb, CompanionRxUnion_Robot, robotOffset.Union());
  FinishSizePrefixedCompanionRxBuffer(fbb, message);
}
