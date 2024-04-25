#include "companion.h"
#include "config.h"
#include <Arduino.h>
using namespace fbs;

void Companion::accept(Message &message) {
  switch (message.type) {
  case ENABLE_WEB_SERVER:
    Serial.println("Enable web server");
    break;
  case DISABLE_WEB_SERVER:
    Serial.println("Disable web server");
    break;
  case STATE_UPDATE:
    Serial.println("State update");
    TaskMessage::State state = message.as.state;
    sendStateToCompanion(state);
    break;
  }
}

void Companion::loop() {
  //  Check for incoming serial messages

  if (companionSerial.available()) {
    // Read first byte to get the message length
    uint8_t messageLength = companionSerial.read();
    // Read message into serialBuffer
    companionSerial.readBytes(serialBuffer, messageLength);
    // TODO Parse flatbuffers message (either a parameter update or a web server
    // enable/disable request)

    if (UpdateBufferHasIdentifier(serialBuffer)) {
      // This is an update message
      auto update = GetUpdate(serialBuffer);
    }
  } else {
  }
}

void Companion::sendTaskMessage(TaskMessage::Message &message) {
  xQueueSend(dataManagerQueue, &message, 0);
}

void Companion::handleUpdateMessage(uint8_t *payload, size_t length) {
  auto update = GetUpdate(payload);
  static TaskMessage::Message taskMessage;
  switch (update->update_type()) {
  case UpdateUnion::UpdateUnion_NONE:
    break;
  case UpdateUnion::UpdateUnion_UpdateBatteries: {
    auto updateBatteries = update->update_as_UpdateBatteries();
    taskMessage = {
        .type = TaskMessage::Type::SET_BATTERY_COUNT,
        .as = {.batteryCount = updateBatteries->batteries()},
    };
    sendTaskMessage(taskMessage);
    break;
  }
  case UpdateUnion::UpdateUnion_UpdateLowVoltageThreshold: {
    auto updateLowVoltageThreshold =
        update->update_as_UpdateLowVoltageThreshold();
    taskMessage = {
        .type = TaskMessage::Type::SET_LOW_VOLTAGE_THRESHOLD,
        .as =
            {
                .voltageThreshold =
                    updateLowVoltageThreshold->low_voltage_threshold(),
            },
    };
    sendTaskMessage(taskMessage);
    break;
  }
  case UpdateUnion::UpdateUnion_UpdateCriticalVoltageThreshold: {
    auto updateCriticalVoltageThreshold =
        update->update_as_UpdateCriticalVoltageThreshold();
    taskMessage = {
        .type = TaskMessage::Type::SET_CRITICAL_VOLTAGE_THRESHOLD,
        .as =
            {
                .voltageThreshold = updateCriticalVoltageThreshold
                                        ->critical_voltage_threshold(),
            },
    };
    sendTaskMessage(taskMessage);
    break;
  }
  case UpdateUnion::UpdateUnion_UpdateReferenceWheelAngle: {
    auto updateReferenceWheelAngle =
        update->update_as_UpdateReferenceWheelAngle();
    // TODO implement
    // robot.reference_wheel_angle =
    // updateReferenceWheelAngle->reference_wheel_angle();
    break;
  }
  case UpdateUnion::UpdateUnion_UpdateFoldWheels: {
    // robot.wheels_folded = true;
    break;
  }
  case UpdateUnion::UpdateUnion_UpdateRssiThreshold:
    break;
  case UpdateUnion::UpdateUnion_UpdateEnableRover:
    break;
  case UpdateUnion::UpdateUnion_UpdateLinkQualityThreshold:
    break;
  }
}

void Companion::companionProducerTask(void *pvParameters) {
  (void)pvParameters; //  To avoid warnings

  companionSerial.begin(115200);

  for (;;) {
    loop();
    // This shouldn't receive messages too often
    vTaskDelay(pdMS_TO_TICKS(128));
  }
}

void Companion::companionConsumerTask(void *pvParameters) {
  (void)pvParameters; //  To avoid warnings
  static Message message;

#if !CFG_ENABLE_COMPANION
  Serial.println("Companion disabled");
  for (;;) {
    xQueueReceive(companionQueue, &message, portMAX_DELAY);
  }
#endif

  for (;;) {
    if (xQueueReceive(companionQueue, &message, portMAX_DELAY)) {
      accept(message);
    }
  }
}

void Companion::sendToCompanion(uint8_t *data, size_t length) {
  // Send the length of the message first
  companionSerial.write(length);
  // Send the message
  companionSerial.write(data, length);
}

void Companion::sendStateToCompanion(TaskMessage::State &state) {
  Serial.println("Sending state to companion");
  serialiseState(state);
  sendToCompanion(fbb.GetBufferPointer(), fbb.GetSize());
}

void Companion::serialiseState(TaskMessage::State &state) {
  fbb.Reset();
  // TODO move these varaibles into TaskMessage::State
  ControlSource control_source = ControlSource_RC;
  Status status = Status_OK;
  float max_speed = 0.0f;
  int reference_wheel_angle = 0;
  std::string motor_error_code;
  bool wheels_folded = false;
  bool enable_rover = false;

  auto motor1 = CreateMotor(fbb, state.motors.left.temperature,
                            state.motors.left.RPM, state.motors.left.position);
  auto motor2 =
      CreateMotor(fbb, state.motors.right.temperature, state.motors.right.RPM,
                  state.motors.right.position);
  auto motors = CreateMotors(fbb, motor1, motor2);

  auto robot = CreateRobot(
      fbb, state.batteryCount, control_source, status, motors,
      state.battery.voltage, state.battery.current, state.battery.fuel,
      0, // TODO fix schema (rssi = signal strength)
      0, max_speed, state.lowVoltageThreshold, state.criticalVoltageThreshold,
      reference_wheel_angle, fbb.CreateString(motor_error_code), wheels_folded,
      enable_rover);

  fbb.Finish(robot);
}
