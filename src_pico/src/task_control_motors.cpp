#include "task_control_motors.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#define MOTOR_SPEED_LIMIT 10.0f
#define MOTOR_CURRENT_LIMIT 5.0f

void initMotors();

void debugAlternateMotorSpeed();

void debugPrintMotorStatus();
void setSpeedIndividual(float speedL, float speedR);

XiaomiCyberGearDriver cybergearL =
    XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_L, MASTER_CAN_ID);
XiaomiCyberGearDriver cybergearR =
    XiaomiCyberGearDriver(CYBERGEAR_CAN_ID_R, MASTER_CAN_ID);

QueueHandle_t controlMotorsQueue;
bool motorsEnabled = false;

void taskControlMotors(void *pvParameters) {
  (void)pvParameters; //  To avoid warnings

  Serial.println("taskControlMotors started");
  controlMotorsQueue = xQueueCreate(10, sizeof(TaskControlMotorsMessage));
  if (controlMotorsQueue == nullptr) {
    Serial.println("Failed to create dataManagerQueue");
    vTaskDelete(nullptr);
  }
  initMotors();

  TaskControlMotorsMessage message;
  for (;;) {
    if (xQueueReceive(controlMotorsQueue, &message, portMAX_DELAY)) {
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
        break;
      case TaskControlMotors::SET_SPEED_INDIVIDUAL:
        setSpeedIndividual(message.speedIndividual.rpmL,
                           message.speedIndividual.rpmR);
        break;
      }


    }
  }
}

void setSpeedIndividual(float speedL, float speedR) {
  cybergearL.set_speed_ref(speedL);
  cybergearR.set_speed_ref(speedR);
}

void initMotors() {
  cybergearL.init_motor(MODE_SPEED);
  cybergearL.set_limit_speed(MOTOR_SPEED_LIMIT);
  cybergearL.set_limit_current(MOTOR_CURRENT_LIMIT);
  cybergearL.enable_motor();
  cybergearR.init_motor(MODE_SPEED);
  cybergearR.set_limit_speed(MOTOR_SPEED_LIMIT);
  cybergearR.set_limit_current(MOTOR_CURRENT_LIMIT);
  cybergearR.enable_motor();
}

void debugAlternateMotorSpeed() {
  cybergearR.set_speed_ref(0);
  cybergearL.set_speed_ref(2);
  delay(1000);
  cybergearL.set_speed_ref(0);
  cybergearR.set_speed_ref(2);
  delay(1000);
}

void debugPrintMotorStatus() {
  XiaomiCyberGearStatus statusL = cybergearL.get_status();
  XiaomiCyberGearStatus statusR = cybergearR.get_status();
  Serial.printf("L: POS:%f V:%f T:%f temp:%d\n", statusL.position,
                statusL.speed, statusL.torque, statusL.temperature);
  Serial.printf("R: POS:%f V:%f T:%f temp:%d\n", statusR.position,
                statusR.speed, statusR.torque, statusR.temperature);
}