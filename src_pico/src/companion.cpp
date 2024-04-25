#include <Arduino.h>
#include "companion.h"
#include "companion_rx_generated.h"
#include "companion_tx_generated.h"
#include "config.h"
using namespace fbs;

void Companion::accept(const Message &message) {
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

void Companion::sendTaskMessage(const TaskMessage::Message &message) {
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
  Serial.println("Companion consumer task started");
  companionQueue = xQueueCreate(10, sizeof(Message));
  if (companionQueue == nullptr) {
    Serial.println("Failed to create companionQueue");
    vTaskDelete(nullptr);
  }
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

void Companion::sendToCompanion(const uint8_t *data, size_t length) {
  // Send the length of the message first
  companionSerial.write(length);
  // Send the message
  companionSerial.write(data, length);
}

void Companion::sendStateToCompanion(const TaskMessage::State &state) {
  Serial.println("Sending state to companion");
  serialiseState(state);
  sendToCompanion(fbb.GetBufferPointer(), fbb.GetSize());
}

void Companion::serialiseState(const TaskMessage::State &state) {
  fbb.Reset();
  RobotT robot = RobotT();
  robot.batteries = state.batteryCount;
  // TODO add these fields:
  robot.control_source = ControlSource_RC;
  robot.status = Status_OK;
  robot.voltage = state.battery.voltage;
  robot.current = state.battery.current;
  robot.fuel = state.battery.fuel;
  robot.rssi = state.rc.rssi;
  robot.link_quality = state.rc.linkQuality;
  // TODO add these fields:
  robot.max_speed = 30;
  robot.low_voltage_threshold = state.lowVoltageThreshold;
  robot.critical_voltage_threshold = state.criticalVoltageThreshold;
  // TODO add these fields:
  //  robot.rssi_threshold = state.rc.rssiThreshold;
  //  robot.link_quality_threshold = state.rc.linkQualityThreshold;
  robot.left_motor_fold_angle = 0;
  robot.right_motor_fold_angle = 0;
  robot.motor_error_code = "";
  robot.enable_rover = true;
  FinishRobotBuffer(fbb, Robot::Pack(fbb, &robot));
}
