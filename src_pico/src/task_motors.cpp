#include "task_motors.h"

#include "FreeRTOS.h"
#include "task.h"

#define MOTOR_SPEED_LIMIT 10.0f
#define MOTOR_CURRENT_LIMIT 5.0f

unsigned long previousMillis = 0; // will store last time a message was send

void initMotors();
void debugAlternateMotorSpeed();
void debugPrintMotorStatus();

void taskMotors(void *pvParameters)
{
    (void)pvParameters; //  To avoid warnings

    for (;;)
    {
        debugPrintMotorStatus();
        debugAlternateMotorSpeed();

        // send a request to the cybergear to receive motor status (position, speed, torque, temperature)
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000)
        {
            previousMillis = currentMillis;
            cybergearL.request_status();
            cybergearR.request_status();
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
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
    cybergearR.set_speed_ref(0);
    cybergearL.set_speed_ref(2);
    delay(1000);
    cybergearL.set_speed_ref(0);
    cybergearR.set_speed_ref(2);
    delay(1000);
}

void debugPrintMotorStatus()
{
    XiaomiCyberGearStatus statusL = cybergearL.get_status();
    XiaomiCyberGearStatus statusR = cybergearR.get_status();
    Serial.printf("L: POS:%f V:%f T:%f temp:%d\n", statusL.position, statusL.speed, statusL.torque, statusL.temperature);
    Serial.printf("R: POS:%f V:%f T:%f temp:%d\n", statusR.position, statusR.speed, statusR.torque, statusR.temperature);
}