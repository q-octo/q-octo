
#include "state_to_flatbuffer.h"
#include "storage.h"

void stateToFlatbuffer(const DataManager::State &robotState, RobotT &robot)
{
  Storage::State &storageState = Storage::getState();
  robot.batteries = storageState.batteryCount;
  // TODO add these fields:
  robot.control_source = ControlSource_RC;
  robot.status = Status_OK;

  robot.voltage = robotState.battery.voltage;
  robot.current = robotState.battery.current;
  robot.fuel = robotState.battery.fuel;
  robot.rssi = robotState.rc.rssi;
  robot.link_quality = robotState.rc.linkQuality;
  // TODO add these fields:
  robot.max_speed = 30;

  robot.low_voltage_threshold = storageState.lowVoltageThreshold;
  robot.critical_voltage_threshold = storageState.criticalVoltageThreshold;
  robot.rssi_threshold = storageState.rssiThreshold;
  robot.link_quality_threshold = storageState.linkQualityThreshold;
  robot.left_motor_fold_angle = storageState.leftMotorFoldAngle;
  robot.right_motor_fold_angle = storageState.rightMotorFoldAngle;
  // TODO add these fields
  robot.motor_error_code = "";
  robot.enable_rover = true;
}