
#include "state_to_flatbuffer.h"
#include "storage.h"
#include "system_status.h"


Status serialiseSystemStatus(const DataManager::Status &systemStatus) {
  switch (systemStatus) {
    case DataManager::OK:
      return Status_OK;
    case DataManager::LOW_BATTERY:
      return Status_LOW_BATTERY;
    case DataManager::NO_TX_SIGNAL:
      return Status_NO_TX_SIGNAL;
    case DataManager::MOTOR_ERROR:
      return Status_MOTOR_ERROR;
    case DataManager::DISARMED:
      return Status_DISARMED;
  }

  // Unreachable
  return Status_OK;
}

void stateToFlatbuffer(const DataManager::State &robotState, RobotT &robot) {
  const Storage::State &storageState = Storage::getState();
  robot.batteries = storageState.batteryCount;
  // TODO get control source
  robot.control_source = ControlSource_RC;
  robot.status = serialiseSystemStatus(SystemStatus::getStatus());

  robot.voltage = robotState.battery.voltage;
  robot.current = robotState.battery.current;
  robot.fuel = robotState.battery.fuel;
  robot.crsf_data->telemetry->link_quality = robotState.rc.linkQuality;
  robot.crsf_data->telemetry->rssi = robotState.rc.rssi;
  robot.crsf_data->telemetry->snr = robotState.rc.signalNoiseRatio;
  robot.crsf_data->telemetry->tx_power = robotState.rc.tx_power;

  ::flatbuffers::span<const uint16_t, 16> data_span(robotState.rc.channels);
  CrsfChannels crsfChannels(data_span);
  robot.crsf_data->channels = std::make_unique<CrsfChannels>(crsfChannels);

  // TODO set these
  robot.crsf_data->failsafe = false;
  robot.max_speed = 30;

  robot.low_voltage_threshold = storageState.lowVoltageThreshold;
  robot.critical_voltage_threshold = storageState.criticalVoltageThreshold;
  robot.rssi_threshold = storageState.rssiThreshold;
  robot.link_quality_threshold = storageState.linkQualityThreshold;
  robot.left_motor_fold_angle = storageState.leftMotorFoldAngle;
  robot.right_motor_fold_angle = storageState.rightMotorFoldAngle;

  // TODO set these
  robot.motor_error_code = "";
  robot.enable_rover = true;

  robot.motors->motor1->angle = robotState.motors.left.position;
  robot.motors->motor1->rps = robotState.motors.left.RPM;
  robot.motors->motor1->torque = robotState.motors.left.torque;
  robot.motors->motor1->temperature = robotState.motors.left.temperature;
  robot.motors->motor2->angle = robotState.motors.right.position;
  robot.motors->motor2->rps = robotState.motors.right.RPM;
  robot.motors->motor2->torque = robotState.motors.right.torque;
  robot.motors->motor2->temperature = robotState.motors.right.temperature;

  robot.display_messages->message1 = *robotState.displayMessages.message1;
  robot.display_messages->message2 = *robotState.displayMessages.message2;
  robot.display_messages->message3 = *robotState.displayMessages.message3;
  robot.display_messages->message4 = *robotState.displayMessages.message4;
  robot.display_messages->message5 = *robotState.displayMessages.message5;
  robot.display_messages->message6 = *robotState.displayMessages.message6;
  robot.display_messages->message7 = *robotState.displayMessages.message7;
}