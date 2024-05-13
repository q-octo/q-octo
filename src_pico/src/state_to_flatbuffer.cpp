
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
  const StorageState &storageState = Storage::getState();
  robot.batteries = storageState.batteryCount;
  // TODO get control source
  robot.control_source = ControlSource_RC;
  robot.status = serialiseSystemStatus(SystemStatus::getStatus());

  robot.voltage = robotState.battery.voltage;
  robot.current = robotState.battery.current;
  robot.fuel = robotState.battery.fuel;
  const CrsfTelemetryT crsfTelemetry = {
    .rssi = robotState.rc.rssi,
    .link_quality = robotState.rc.linkQuality,
    .snr = robotState.rc.signalNoiseRatio,
    .tx_power = robotState.rc.tx_power
  };
  CrsfChannelsT crsfChannels{};
  if (robotState.rc.channels != nullptr) {
    crsfChannels.c1 = robotState.rc.channels[0];
    crsfChannels.c2 = robotState.rc.channels[1];
    crsfChannels.c3 = robotState.rc.channels[2];
    crsfChannels.c4 = robotState.rc.channels[3];
    crsfChannels.c5 = robotState.rc.channels[4];
    crsfChannels.c6 = robotState.rc.channels[5];
    crsfChannels.c7 = robotState.rc.channels[6];
    crsfChannels.c8 = robotState.rc.channels[7];
    crsfChannels.c9 = robotState.rc.channels[8];
    crsfChannels.c10 = robotState.rc.channels[9];
    crsfChannels.c11 = robotState.rc.channels[10];
    crsfChannels.c12 = robotState.rc.channels[11];
    crsfChannels.c13 = robotState.rc.channels[12];
    crsfChannels.c14 = robotState.rc.channels[13];
    crsfChannels.c15 = robotState.rc.channels[14];
    crsfChannels.c16 = robotState.rc.channels[15];
  }

  CrsfDataT crsfData{};
  crsfData.telemetry = std::make_unique<CrsfTelemetryT>(crsfTelemetry);
  crsfData.channels = std::make_unique<CrsfChannelsT>(crsfChannels);
  crsfData.failsafe = robotState.rc.failsafe;
  robot.crsf_data = std::make_unique<CrsfDataT>(crsfData);

  robot.low_voltage_threshold = storageState.lowVoltageThreshold;
  robot.critical_voltage_threshold = storageState.criticalVoltageThreshold;
  robot.rssi_threshold = storageState.rssiThreshold;
  robot.link_quality_threshold = storageState.linkQualityThreshold;
  robot.left_motor_fold_angle = storageState.leftMotorFoldAngle;
  robot.right_motor_fold_angle = storageState.rightMotorFoldAngle;
  
  robot.start_web_server_on_launch = storageState.startWebServerOnLaunch;
  robot.web_server_enabled = robotState.webServerEnabled;
  
  // TODO set these
  robot.motor_error_code = "";
  robot.enable_rover = true;

  const MotorT leftMotor = {
    .temperature = robotState.motors.left.temperature,
    .rps = robotState.motors.left.RPM,
    .angle = robotState.motors.left.position,
    .torque = robotState.motors.left.torque,
  };
  const MotorT rightMotor = {
    .temperature = robotState.motors.right.temperature,
    .rps = robotState.motors.right.RPM,
    .angle = robotState.motors.right.position,
    .torque = robotState.motors.right.torque,
  };

  MotorsT motors{};
  motors.motor1 = std::make_unique<MotorT>(leftMotor);
  motors.motor2 = std::make_unique<MotorT>(rightMotor);
  robot.motors = std::make_unique<MotorsT>(motors);
  
  DisplayMessagesT displayMessages{};
  displayMessages.message1 = *robotState.displayMessages.message1;
  displayMessages.message2 = *robotState.displayMessages.message2;
  displayMessages.message3 = *robotState.displayMessages.message3;
  displayMessages.message4 = *robotState.displayMessages.message4;
  displayMessages.message5 = *robotState.displayMessages.message5;
  displayMessages.message6 = *robotState.displayMessages.message6;
  displayMessages.message7 = *robotState.displayMessages.message7;

  robot.display_messages = std::make_unique<DisplayMessagesT>(displayMessages);

  MotorLimitsT leftMotorLimits{
    .max_speed = robotState.leftMotorLimits.last_max_speed_update > 0 ? robotState.leftMotorLimits.max_speed : -1,
    .max_current = robotState.leftMotorLimits.last_max_current_update > 0 ? robotState.leftMotorLimits.max_current : -1,
    .max_torque = robotState.leftMotorLimits.last_max_torque_update > 0 ? robotState.leftMotorLimits.max_torque : -1,
    .speed_kp = robotState.leftMotorLimits.last_speed_kp_update > 0 ? robotState.leftMotorLimits.speed_kp : -1,
    .speed_ki = robotState.leftMotorLimits.last_speed_ki_update > 0 ? robotState.leftMotorLimits.speed_ki : -1,
  };
  MotorLimitsT rightMotorLimits{
    .max_speed = robotState.rightMotorLimits.last_max_speed_update > 0 ? robotState.rightMotorLimits.max_speed : -1,
    .max_current = robotState.rightMotorLimits.last_max_current_update > 0 ? robotState.rightMotorLimits.max_current : -1,
    .max_torque = robotState.rightMotorLimits.last_max_torque_update > 0 ? robotState.rightMotorLimits.max_torque : -1,
    .speed_kp = robotState.rightMotorLimits.last_speed_kp_update > 0 ? robotState.rightMotorLimits.speed_kp : -1,
    .speed_ki = robotState.rightMotorLimits.last_speed_ki_update > 0 ? robotState.rightMotorLimits.speed_ki : -1,
  };
  robot.left_motor_limits = std::make_unique<MotorLimitsT>(leftMotorLimits);
  robot.right_motor_limits = std::make_unique<MotorLimitsT>(rightMotorLimits);
  
}