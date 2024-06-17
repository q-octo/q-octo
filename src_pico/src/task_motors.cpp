/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "config.h"
#include "task_motors.h"
#include "storage.h"
#include "task_can.h"

#define STATUS_BROADCAST_FREQUENCY 3000 // ms

void TaskControlMotors::init() {
  if (!CFG_ENABLE_MOTORS) {
    return;
  }

  StorageState &state = Storage::getState();
  maxSpeed = state.motorSpeedLimit;
  maxCurrent = state.motorCurrentLimit;
  maxTorque = state.motorTorqueLimit;
  speedKi = state.speedKi;
  speedKp = state.speedKp;

  leftMotorLimits = {
    .last_max_speed_update = 0,
    .last_max_current_update = 0,
    .last_max_torque_update = 0,
    .last_speed_kp_update = 0,
    .last_speed_ki_update = 0,
  };

  rightMotorLimits = {
    .last_max_speed_update = 0,
    .last_max_current_update = 0,
    .last_max_torque_update = 0,
    .last_speed_kp_update = 0,
    .last_speed_ki_update = 0,
  };

  initMotors();
  Serial.println("Motor class init complete");
}

void TaskControlMotors::loop() {
  if (!CFG_ENABLE_MOTORS) {
    return;
  }
  const uint32_t currentMillis = millis();
  const uint32_t currentMicros = micros();

  if (currentMillis - lastStatusBroadcastMs > STATUS_BROADCAST_FREQUENCY) {
    broadcastStatusUpdate();
    lastStatusBroadcastMs = currentMillis;
  }
  auto leftParams = cybergearL.get_motor_param();
  auto rightParams = cybergearR.get_motor_param();
  static DataManager::Message message;
  if (leftParams.stamp_usec > lastLeftMotorParameterResponseMicros) {
    lastLeftMotorParameterResponseMicros = leftParams.stamp_usec;
    leftMotorLimits = {
      .max_speed = leftParams.limit_spd,
      .max_current = leftParams.limit_cur,
      .max_torque = leftParams.limit_torque,
      .speed_kp = leftParams.spd_kp,
      .speed_ki = leftParams.spd_ki,
      .last_max_speed_update = leftParams.last_speed_update_usec,
      .last_max_current_update = leftParams.last_current_update_usec,
      .last_max_torque_update = leftParams.last_torque_update_usec,
      .last_speed_kp_update = leftParams.last_speed_kp_update_usec,
      .last_speed_ki_update = leftParams.last_speed_ki_update_usec,
    };
    message.type = DataManager::Type::LEFT_MOTOR_PARAM_UPDATED;
    message.as.motorParams = leftMotorLimits;
    DataManager::receiveMessage(message);
  }

  if (rightParams.stamp_usec > lastRightMotorParameterResponseMicros) {
    lastRightMotorParameterResponseMicros = rightParams.stamp_usec;
    rightMotorLimits = {
      .max_speed = rightParams.limit_spd,
      .max_current = rightParams.limit_cur,
      .max_torque = rightParams.limit_torque,
      .speed_kp = rightParams.spd_kp,
      .speed_ki = rightParams.spd_ki,
      .last_max_speed_update = rightParams.last_speed_update_usec,
      .last_max_current_update = rightParams.last_current_update_usec,
      .last_max_torque_update = rightParams.last_torque_update_usec,
      .last_speed_kp_update = rightParams.last_speed_kp_update_usec,
      .last_speed_ki_update = rightParams.last_speed_ki_update_usec,
    };
    message.type = DataManager::Type::RIGHT_MOTOR_PARAM_UPDATED;
    message.as.motorParams = rightMotorLimits;
    DataManager::receiveMessage(message);
  } 


  if (currentMicros - lastSpeedUpdateMicros > MOTOR_SPEED_UPDATE_INTERVAL_MICROS) {
    lastSpeedUpdateMicros = currentMicros;
    pendingSpeedUpdate = false;

    // setCurrentLimit(maxCurrent);
    // delayMicroseconds(MOTOR_COMMAND_DELAY_MICROS);
    // setTorqueLimit(maxTorque);
    // delayMicroseconds(MOTOR_COMMAND_DELAY_MICROS); 
    cybergearL.set_speed_ref(motorsEnabled ? lastSpeedL : 0);
    checkCAN();
    cybergearR.set_speed_ref(motorsEnabled ? lastSpeedR : 0);
    checkCAN();

    // cybergearL.get_current_limit();
    // cybergearL.get_vbus();
  }

  if (currentMillis - lastParameterRequestMillis > 2000) {
    lastParameterRequestMillis = currentMillis;
    getSpeedLimit();
    getCurrentLimit();
    getTorqueLimit();
    getSpeedKi();
    getSpeedKp(); 
  }

}

void TaskControlMotors::getSpeedLimit() {
  cybergearL.get_speed_limit();
  checkCAN();
  cybergearR.get_speed_limit();
  checkCAN();
}

void TaskControlMotors::getCurrentLimit() {
  cybergearL.get_current_limit();
  checkCAN();
  cybergearR.get_current_limit();
  checkCAN();
}

void TaskControlMotors::getTorqueLimit() {
  cybergearL.get_torque_limit();
  checkCAN();
  cybergearR.get_torque_limit();
  checkCAN();
}

void TaskControlMotors::getSpeedKi() {
  cybergearL.get_speed_ki();
  checkCAN();
  cybergearR.get_speed_ki();
  checkCAN();
}

void TaskControlMotors::getSpeedKp() {
  cybergearL.get_speed_kp();
  checkCAN();
  cybergearR.get_speed_kp();
  checkCAN();
}

void TaskControlMotors::checkCAN() {
  // The can board seems to only buffer a limited number of frames.
  // So earlier responses will be lost if we send consecutive motor 
  // instructions unless we check for them here.
  TaskCAN::loop();
}

void TaskControlMotors::handleStateUpdate() {
    StorageState &state = Storage::getState();
    if (state.motorSpeedLimit != maxSpeed) {
      maxSpeed = state.motorSpeedLimit;
      setSpeedLimit(maxSpeed); 
      Serial.print("Speed limit: ");
      Serial.println(maxSpeed);
    }
    if (state.motorCurrentLimit != maxCurrent) {
      maxCurrent = state.motorCurrentLimit;
      setCurrentLimit(maxCurrent);
      Serial.print("Current limit: ");
      Serial.println(maxCurrent);
    }
    if (state.motorTorqueLimit != maxTorque) {
      maxTorque = state.motorTorqueLimit;
      setTorqueLimit(maxTorque);
      Serial.print("Torque limit: ");
      Serial.println(maxTorque);
    } 
    if (state.speedKi != speedKi) {
      speedKi = state.speedKi;
      setSpeedKi(speedKi);
      Serial.print("Speed Ki: ");
      Serial.println(speedKi);
    }
    if (state.speedKp != speedKp) {
      speedKp = state.speedKp;
      setSpeedKp(speedKp);
      Serial.print("Speed Kp: ");
      Serial.println(speedKp);
    }
}

void TaskControlMotors::receiveMessage(const TaskControlMotors::Message &message) {
  if (!CFG_ENABLE_MOTORS) {
    return;
  }

  switch (message.type) {
    case TaskControlMotors::ENABLE:
      motorsEnabled = true;
      cybergearL.enable_motor();
      checkCAN();
      cybergearR.enable_motor();
      checkCAN();
      break;
    case TaskControlMotors::DISABLE:
      motorsEnabled = false;
      lastSpeedL = 0;
      lastSpeedR = 0;
      cybergearL.stop_motor();
      checkCAN();
      cybergearR.stop_motor();
      checkCAN();
      break;
    case TaskControlMotors::SET_SPEED_COMBINED:
      setSpeedCombined(message.as.speedCombined.speed,
                       message.as.speedCombined.direction);
      break;
    case TaskControlMotors::SET_SPEED_INDIVIDUAL:
      setSpeedIndividual(message.as.speedIndividual.leftSpeed,
                         message.as.speedIndividual.rightSpeed);
      break;
    case TaskControlMotors::CAN_MESSAGE_MOTOR_L:
      cybergearL.process_message(message.as.canMessage.id,
                                 message.as.canMessage.data);
      break;
    case TaskControlMotors::CAN_MESSAGE_MOTOR_R:
      cybergearR.process_message(message.as.canMessage.id,
                                 message.as.canMessage.data);
      break;
    case TaskControlMotors::FOLD_WHEELS:
      // TODO implement folding
      break;
    case STATE_UPDATE:
      handleStateUpdate();
      break;
    case UPDATE_SPEED_LIMIT:
      setSpeedLimit(message.as.floatMotorParam);
      break;
    case UPDATE_CURRENT_LIMIT:
      setCurrentLimit(message.as.floatMotorParam);
      break;
    case UPDATE_TORQUE_LIMIT:
      setTorqueLimit(message.as.floatMotorParam);
      break;
    case UPDATE_SPEED_KI:
      setSpeedKi(message.as.floatMotorParam);
      break;
    case UPDATE_SPEED_KP:
      setSpeedKp(message.as.floatMotorParam); 
      break;
  }
}

void TaskControlMotors::setSpeedLimit(float speedLimit) {
  cybergearL.set_limit_speed(speedLimit);
  checkCAN();
  cybergearR.set_limit_speed(speedLimit);
  checkCAN();
  getSpeedLimit(); 
}

void TaskControlMotors::setCurrentLimit(float currentLimit) {
  cybergearL.set_limit_current(currentLimit);
  checkCAN();
  cybergearR.set_limit_current(currentLimit);
  checkCAN();
  getCurrentLimit();
}

void TaskControlMotors::setTorqueLimit(float torqueLimit) {
  cybergearL.set_limit_torque(torqueLimit);
  checkCAN();
  cybergearR.set_limit_torque(torqueLimit);
  checkCAN();
  getTorqueLimit();
}

void TaskControlMotors::setSpeedKi(float speedKi) {
  cybergearL.set_speed_ki(speedKi);
  checkCAN();
  cybergearR.set_speed_ki(speedKi);
  checkCAN();
  getSpeedKi();
}

void TaskControlMotors::setSpeedKp(float speedKp) {
  cybergearL.set_speed_kp(speedKp);
  checkCAN();
  cybergearR.set_speed_kp(speedKp);
  checkCAN();
  getSpeedKp();
}

void TaskControlMotors::broadcastStatusUpdate() {
  auto statusL = cybergearL.get_status();
  auto statusR = cybergearR.get_status();
  DataManager::Motors motors = {
          .left =
                  {
                          .temperature = statusL.temperature / 10.0f,
                          // Negated
                          .RPM = -statusL.speed,
                          .torque = statusL.torque,
                          .position = statusL.position,
                  },
          .right =
                  {
                          .temperature = statusR.temperature / 10.0f,
                          .RPM = statusR.speed,
                          .torque = statusR.torque,
                          .position = statusR.position,
                  },
  };
  DataManager::Message message = {.type = DataManager::Type::STATE_MOTORS,
          .as = {.motors = motors}};
  DataManager::receiveMessage(message);
}

void TaskControlMotors::setSpeedIndividual(float speedL, float speedR) {
  // Serial.printf("Setting speed L: %f R: %f\n", speedL, speedR);
  
  if (abs(maxSpeed) > 30 || abs(maxSpeed) > 30) {
    Serial.println("[WARN] max speed is configured out of range!");
    Serial.println("Not setting speed.");
    return;
  }
  
  // These checks are important because the motor doesn't have a speed limit
  // that applies to the speed control mode.
  if (speedL > maxSpeed || speedL < -maxSpeed) {
    Serial.println("[WARN] Speed limit exceeded for left motor");
    speedL = speedL > 0 ? maxSpeed : -maxSpeed;
  } 
  if (speedR > maxSpeed || speedR < -maxSpeed) {
    Serial.println("[WARN] Speed limit exceeded for right motor");
    speedR = speedR > 0 ? maxSpeed : -maxSpeed;
  }
  lastSpeedL = speedL;
  lastSpeedR = speedR;
  pendingSpeedUpdate = true;
}

void TaskControlMotors::setSpeedCombined(float speed, float direction) {
    // Speed is between -30 and 30 rad/s
    // Direction is between -1 and 1 (full left to full right)
    float scaledSpeed = speed / maxSpeed;
    float left = (scaledSpeed + direction) * maxSpeed;
    float right = (scaledSpeed - direction) * maxSpeed;
    setSpeedIndividual(-left, right);
}

void TaskControlMotors::initMotors() {
  // Can this actually work given that we don't check for can messages?
  cybergearL.init_motor(MODE_SPEED);
  checkCAN();
  cybergearR.init_motor(MODE_SPEED);
  checkCAN();
  setSpeedLimit(maxSpeed);
  setCurrentLimit(maxCurrent);
  setTorqueLimit(maxTorque);
  setSpeedKi(speedKi);
  setSpeedKp(speedKp);
  cybergearL.enable_motor();
  checkCAN();
  cybergearR.enable_motor();
  checkCAN();
}

void TaskControlMotors::debugPrintMotorStatus() {
  XiaomiCyberGearStatus statusL = cybergearL.get_status();
  XiaomiCyberGearStatus statusR = cybergearR.get_status();
  Serial.printf("L: POS:%f V:%f T:%f temp:%d\n", statusL.position,
                statusL.speed, statusL.torque, statusL.temperature);
  Serial.printf("R: POS:%f V:%f T:%f temp:%d\n", statusR.position,
                statusR.speed, statusR.torque, statusR.temperature);
}