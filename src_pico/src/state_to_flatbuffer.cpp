
#include "state_to_flatbuffer.h"

void stateToFlatbuffer(const DataManager::State &robotState, RobotT &robot)
{
  robot.batteries = robotState.batteryCount;
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
  robot.low_voltage_threshold = robotState.lowVoltageThreshold;
  robot.critical_voltage_threshold = robotState.criticalVoltageThreshold;
  // TODO add these fields:
  //  robot.rssi_threshold = state.rc.rssiThreshold;
  //  robot.link_quality_threshold = state.rc.linkQualityThreshold;
  robot.left_motor_fold_angle = 0;
  robot.right_motor_fold_angle = 0;
  robot.motor_error_code = "";
  robot.enable_rover = true;
}