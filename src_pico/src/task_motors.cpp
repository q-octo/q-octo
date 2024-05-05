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
    }
    if (state.motorCurrentLimit != maxCurrent) {
      maxCurrent = state.motorCurrentLimit;
      cybergearL.set_limit_current(maxCurrent);
      cybergearR.set_limit_current(maxCurrent);
    }
    if (state.motorTorqueLimit != maxTorque) {
      maxTorque = state.motorTorqueLimit;
      cybergearL.set_limit_torque(maxTorque);
      cybergearR.set_limit_torque(maxTorque);
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
      setSpeedCombined(message.as.speedCombined.rpm,
                       message.as.speedCombined.direction);
      break;
    case TaskControlMotors::SET_SPEED_INDIVIDUAL:
      setSpeedIndividual(message.as.speedIndividual.rpmL,
                         message.as.speedIndividual.rpmR);
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
                          .temperature = statusL.temperature,
                          .RPM = statusL.speed,
                          .torque = statusL.torque,
                          .position = statusL.position,
                  },
          .right =
                  {
                          // .temperature = statusR.temperature,
                          .temperature = static_cast<uint16_t>((millis() % 24)),
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
      // TODO implement mixing
  setSpeedIndividual(speed, -speed);
}

void TaskControlMotors::initMotors() {
  // Command + 3 x ram write + command (for each motor)
  // Seems like we only get two responses back though, so are we sending
  // the messages too fast? The motor needs 500us between messages
  cybergearL.stop_motor();
  cybergearL.set_run_mode(MODE_SPEED);
  cybergearL.set_limit_speed(maxSpeed);
  cybergearL.set_limit_current(maxCurrent);
  cybergearL.set_limit_torque(maxTorque);
  cybergearL.enable_motor();

  cybergearR.stop_motor();
  cybergearR.set_run_mode(MODE_SPEED);
  cybergearR.set_limit_speed(maxSpeed);
  cybergearR.set_limit_current(maxCurrent);
  cybergearR.set_limit_torque(maxTorque);
  cybergearR.enable_motor();
  Serial.println("Motors initialised");
  Serial.print("Speed limit: ");
  Serial.println(maxSpeed);
}

void TaskControlMotors::debugPrintMotorStatus() {
  XiaomiCyberGearStatus statusL = cybergearL.get_status();
  XiaomiCyberGearStatus statusR = cybergearR.get_status();
  Serial.printf("L: POS:%f V:%f T:%f temp:%d\n", statusL.position,
                statusL.speed, statusL.torque, statusL.temperature);
  Serial.printf("R: POS:%f V:%f T:%f temp:%d\n", statusR.position,
                statusR.speed, statusR.torque, statusR.temperature);
}