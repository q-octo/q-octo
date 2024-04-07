#include "config.h"
#include "task_motors.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#define MOTOR_SPEED_LIMIT 10.0f
#define MOTOR_CURRENT_LIMIT 5.0f
#define STATUS_REQUEST_FREQUENCY 500   // ms
#define STATUS_BROADCAST_FREQUENCY 500 // ms

void initMotors();

void debugAlternateMotorSpeed();

void debugPrintMotorStatus();
void setSpeedIndividual(float speedL, float speedR);

XiaomiCyberGearDriver cybergearL =
    XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_L, MASTER_CAN_ID);
XiaomiCyberGearDriver cybergearR =
    XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_R, MASTER_CAN_ID);

QueueHandle_t controlMotorsQueue;
bool motorsEnabled = true;

uint32_t lastStatusRequestMs = 0;
uint32_t lastStatusBroadcastMs = 0;

void taskControlMotors(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings

  Serial.println("taskControlMotors started");
  controlMotorsQueue = xQueueCreate(10, sizeof(TaskControlMotors::Message));
  if (controlMotorsQueue == nullptr)
  {
    Serial.println("Failed to create dataManagerQueue");
    vTaskDelete(nullptr);
  }
#if !CFG_ENABLE_MOTORS
  Serial.println("Motors disabled, blocking indefinitely");
  for (;;)
  {
    TaskControlMotors::Message message;
    xQueueReceive(controlMotorsQueue, &message, portMAX_DELAY);
  }
#endif

  initMotors();

  TaskControlMotors::Message message;
  for (;;)
  {
    if (xQueueReceive(controlMotorsQueue, &message, portMAX_DELAY))
    {
      switch (message.type)
      {
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
        // TODO implement mixing
        setSpeedIndividual(message.as.speedCombined.rpm,
                           message.as.speedCombined.rpm);
        break;
      case TaskControlMotors::SET_SPEED_INDIVIDUAL:
        setSpeedIndividual(message.as.speedIndividual.rpmL,
                           message.as.speedIndividual.rpmR);
        break;
      case TaskControlMotors::CAN_MESSAGE_MOTOR_L:
        cybergearL.process_message(message.as.canMessage.data);
        break;
      case TaskControlMotors::CAN_MESSAGE_MOTOR_R:
        cybergearR.process_message(message.as.canMessage.data);
        break;
      }
    }
  }
}

void taskMotors(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  Serial.println("taskMotors started");
  
  for (;;) {
    setSpeedIndividual(0, 2);
    vTaskDelay(pdMS_TO_TICKS(1000));
    setSpeedIndividual(2, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  for (;;)
  {
    // send a request to the cybergear to receive motor status (position, speed,
    // torque, temperature)
    const uint32_t currentMillis = millis();
    if (currentMillis - lastStatusRequestMs >= STATUS_REQUEST_FREQUENCY)
    {
      lastStatusRequestMs = currentMillis;
      cybergearL.request_status();
      cybergearR.request_status();
    }

    if (currentMillis - lastStatusBroadcastMs >= STATUS_BROADCAST_FREQUENCY)
    {
      lastStatusBroadcastMs = currentMillis;
      auto statusL = cybergearL.get_status();
      auto statusR = cybergearR.get_status();
      TaskMessage::Motors motors = {
          .left =
              {
                  .temperature = statusL.temperature,
                  .RPM = statusL.speed,
                  .torque = statusL.torque,
                  .position = statusL.position,
              },
          .right =
              {
                  //                  .temperature = statusR.temperature,
                  .temperature = static_cast<uint16_t>((currentMillis % 24)),
                  .RPM = statusR.speed,
                  .torque = statusR.torque,
                  .position = statusR.position,
              },
      };
      TaskMessage::Message message = {.type = TaskMessage::Type::STATE_MOTORS,
                                      .as = {.motors = motors}};
      xQueueSend(dataManagerQueue, &message, 0);
    }

    // When the motor CAN data was monitored via a saleae logic analyzer, it was
    // sending a constant stream of messages. Do we receive these messages, are
    // they status updates? If so, we could avoid this polling approach
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void setSpeedIndividual(float speedL, float speedR)
{
  Serial.printf("Setting speed L: %f R: %f\n", speedL, speedR);
  cybergearL.set_speed_ref(speedL);
  cybergearR.set_speed_ref(speedR);
}

void initMotors()
{
  cybergearL.init_motor(MODE_SPEED);
  cybergearL.set_limit_speed(MOTOR_SPEED_LIMIT);
  cybergearL.set_limit_current(MOTOR_CURRENT_LIMIT);
  cybergearL.enable_motor();
  cybergearR.init_motor(MODE_SPEED);
  cybergearR.set_limit_speed(MOTOR_SPEED_LIMIT);
  cybergearR.set_limit_current(MOTOR_CURRENT_LIMIT);
  cybergearR.enable_motor();
}

void debugAlternateMotorSpeed()
{
  Serial.println("L: 0 R: 2");
  cybergearR.set_speed_ref(0);
  cybergearL.set_speed_ref(2);
  delayMicroseconds(1000 * 1000);
  Serial.println("L: 2 R: 0");
  cybergearL.set_speed_ref(0);
  cybergearR.set_speed_ref(2);
  delayMicroseconds(1000 * 1000);
}

void debugPrintMotorStatus()
{
  XiaomiCyberGearStatus statusL = cybergearL.get_status();
  XiaomiCyberGearStatus statusR = cybergearR.get_status();
  Serial.printf("L: POS:%f V:%f T:%f temp:%d\n", statusL.position,
                statusL.speed, statusL.torque, statusL.temperature);
  Serial.printf("R: POS:%f V:%f T:%f temp:%d\n", statusR.position,
                statusR.speed, statusR.torque, statusR.temperature);
}