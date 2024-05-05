#include "config.h"
#include "task_motors.h"
#include "storage.h"

#define STATUS_BROADCAST_FREQUENCY 20 // ms

void TaskControlMotors::init() {
  if (!CFG_ENABLE_MOTORS) {
    return;
  }

  Storage::State &state = Storage::getState();
  maxSpeed = state.motorSpeedLimit;
  maxCurrent = state.motorCurrentLimit;
  maxTorque = state.motorTorqueLimit;

  initMotors();
  Serial.println("Motor class init complete");
}

void TaskControlMotors::handleStateUpdate() {
    Storage::State &state = Storage::getState();
    if (state.motorSpeedLimit != maxSpeed) {
      maxSpeed = state.motorSpeedLimit;
      cybergearL.set_limit_speed(maxSpeed);
      cybergearR.set_limit_speed(maxSpeed);
      Serial.print("Speed limit: ");
      Serial.println(maxSpeed);
    }
    if (state.motorCurrentLimit != maxCurrent) {
      maxCurrent = state.motorCurrentLimit;
      cybergearL.set_limit_current(maxCurrent);
      cybergearR.set_limit_current(maxCurrent);
      cybergearR.set_limit_current(maxCurrent);
      Serial.print("Current limit: ");
      Serial.println(maxCurrent);
    }
    if (state.motorTorqueLimit != maxTorque) {
      maxTorque = state.motorTorqueLimit;
      cybergearL.set_limit_torque(maxTorque);
      cybergearR.set_limit_torque(maxTorque);
      Serial.print("Torque limit: ");
      Serial.println(maxTorque);
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
      cybergearR.enable_motor();
      break;
    case TaskControlMotors::DISABLE:
      motorsEnabled = false;
      cybergearL.stop_motor();
      cybergearR.stop_motor();
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
  }
}

void TaskControlMotors::broadcastStatusUpdate() {
  auto statusL = cybergearL.get_status();
  auto statusR = cybergearR.get_status();
  DataManager::Motors motors = {
          .left =
                  {
                          .temperature = statusL.temperature / 10.0f,
                          .RPM = statusL.speed,
                          .torque = statusL.torque,
                          .position = statusL.position,
                  },
          .right =
                  {
                          // .temperature = statusR.temperature / 10.0f,
                          .temperature = static_cast<float>((millis() % 24)),
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
  // cybergearL.enable_motor();
  cybergearL.set_speed_ref(speedL);
  cybergearR.set_speed_ref(speedR);
}

void TaskControlMotors::setSpeedCombined(float speed, float direction) {
    // Speed is between -30 and 30 rad/s
    // Direction is between -1 and 1 (full left to full right)
    float scaledSpeed = speed / maxSpeed;
    float left = (scaledSpeed - direction) * maxSpeed;
    float right = (scaledSpeed + direction) * maxSpeed;
    setSpeedIndividual(left, -right);
}

void TaskControlMotors::initMotors() {
  cybergearL.init_motor(MODE_SPEED);
  cybergearL.set_limit_speed(maxSpeed);
  cybergearL.set_limit_current(maxCurrent);
  cybergearL.set_limit_torque(maxTorque);
  cybergearL.enable_motor();

  cybergearR.init_motor(MODE_SPEED);
  cybergearR.set_limit_speed(maxSpeed);
  cybergearR.set_limit_current(maxCurrent);
  cybergearR.set_limit_torque(maxTorque);
  cybergearR.enable_motor();
  Serial.println("Motors initialised");
  Serial.print("Speed limit: ");
  Serial.println(maxSpeed);
  Serial.print("Current limit: ");
  Serial.println(maxCurrent);
  Serial.print("Torque limit: ");
  Serial.println(maxTorque);
}

void TaskControlMotors::debugPrintMotorStatus() {
  XiaomiCyberGearStatus statusL = cybergearL.get_status();
  XiaomiCyberGearStatus statusR = cybergearR.get_status();
  Serial.printf("L: POS:%f V:%f T:%f temp:%d\n", statusL.position,
                statusL.speed, statusL.torque, statusL.temperature);
  Serial.printf("R: POS:%f V:%f T:%f temp:%d\n", statusR.position,
                statusR.speed, statusR.torque, statusR.temperature);
}