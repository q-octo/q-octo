#include <Arduino.h>
#include "companion.h"
#include "config.h"

void Companion::receiveMessage(const Message &message)
{
#if !CFG_ENABLE_COMPANION
  return;
#endif
  switch (message.type)
  {
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

void Companion::loop()
{
  parseIncomingSerialData();
}

bool Companion::verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier)
{
  return SizePrefixedCompanionTxBufferHasIdentifier(fbSerialParser.buffer) &&
         VerifySizePrefixedCompanionTxBuffer(verifier);
}

void Companion::parseIncomingSerialData()
{
  if (!fbSerialParser.parseMessage())
  {
    return;
  }
  auto companionMessage = GetSizePrefixedCompanionTx(fbSerialParser.buffer);
  handleCompanionTx(*companionMessage);
}

void Companion::handleCompanionTx(const CompanionTx &companionMessage)
{
  switch (companionMessage.message_type())
  {
  case CompanionTxUnion::CompanionTxUnion_NONE:
    break;
  case CompanionTxUnion::CompanionTxUnion_Update:
  {
    auto update = companionMessage.message_as_Update();
    handleUpdateMessage(*update);
    break;
  }
  case CompanionTxUnion::CompanionTxUnion_ButtonPressed:
  {
    auto buttonPressed = companionMessage.message_as_ButtonPressed();
    handleButtonPressedMessage(*buttonPressed);
    break;
  }
  }
}

void Companion::sendTaskMessage(const TaskMessage::Message &message)
{
  xQueueSend(dataManagerQueue, &message, 0);
}

void Companion::handleUpdateMessage(const Update &update)
{
  static TaskMessage::Message taskMessage;
  switch (update.update_type())
  {
  case UpdateUnion::UpdateUnion_NONE:
    break;
  case UpdateUnion::UpdateUnion_UpdateBatteries:
  {
    auto updateBatteries = update.update_as_UpdateBatteries();
    taskMessage = {
        .type = TaskMessage::Type::SET_BATTERY_COUNT,
        .as = {.batteryCount = updateBatteries->batteries()},
    };
    sendTaskMessage(taskMessage);
    break;
  }
  case UpdateUnion::UpdateUnion_UpdateLowVoltageThreshold:
  {
    auto updateLowVoltageThreshold =
        update.update_as_UpdateLowVoltageThreshold();
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
  case UpdateUnion::UpdateUnion_UpdateCriticalVoltageThreshold:
  {
    auto updateCriticalVoltageThreshold =
        update.update_as_UpdateCriticalVoltageThreshold();
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
  case UpdateUnion::UpdateUnion_UpdateReferenceWheelAngle:
  {
    auto updateReferenceWheelAngle =
        update.update_as_UpdateReferenceWheelAngle();
    // TODO implement
    // robot.reference_wheel_angle =
    // updateReferenceWheelAngle->reference_wheel_angle();
    break;
  }
  case UpdateUnion::UpdateUnion_UpdateFoldWheels:
  {
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

void Companion::handleButtonPressedMessage(const ButtonPressed &buttonPressed)
{
  static TaskMessage::Message taskMessage;
  switch (buttonPressed.button())
  {
  case Button_A:
    taskMessage = {
        .type = TaskMessage::Type::DISPLAY_BUTTON_PRESSED,
        .as = {.displayButton = TaskMessage::DisplayButton::A}};
    sendTaskMessage(taskMessage);
    break;
  case Button_B:
    taskMessage = {
        .type = TaskMessage::Type::DISPLAY_BUTTON_PRESSED,
        .as = {.displayButton = TaskMessage::DisplayButton::B}};
    sendTaskMessage(taskMessage);
    break;
  case Button_X:
    // Toggle wifi
    taskMessage = {.type = TaskMessage::Type::TOGGLE_WEB_SERVER_ENABLED};
    sendTaskMessage(taskMessage);
    break;
  }
}

void Companion::init()
{
  companionSerial.begin(115200);
}

void Companion::task(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  init();

  for (;;)
  {
    loop();
    // This shouldn't receive messages too often
    vTaskDelay(pdMS_TO_TICKS(128));
  }
}

void Companion::taskConsumer(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  Serial.println("Companion consumer task started");
  companionQueue = xQueueCreate(10, sizeof(Message));
  if (companionQueue == nullptr)
  {
    Serial.println("Failed to create companionQueue");
    vTaskDelete(nullptr);
  }
  static Message message;

#if !CFG_ENABLE_COMPANION
  Serial.println("Companion disabled");
  for (;;)
  {
    xQueueReceive(companionQueue, &message, portMAX_DELAY);
  }
#endif

  for (;;)
  {
    if (xQueueReceive(companionQueue, &message, portMAX_DELAY))
    {
      receiveMessage(message);
    }
  }
}

void Companion::sendStateToCompanion(const TaskMessage::State &state)
{
  Serial.println("Sending state to companion");
  serialiseState(state);
  companionSerial.write(fbb.GetBufferPointer(), fbb.GetSize());
}

void Companion::serialiseState(const TaskMessage::State &state)
{
  fbb.Reset();

  RobotT robot = RobotT();
  stateToFlatbuffer(state, robot);

  auto robotOffset = CreateRobot(fbb, &robot);
  auto message = CreateCompanionRx(fbb, CompanionRxUnion_Robot, robotOffset.Union());
  FinishSizePrefixedCompanionRxBuffer(fbb, message);
}
