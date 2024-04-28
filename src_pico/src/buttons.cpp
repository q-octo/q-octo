#include "buttons.h"
#include <Arduino.h>
#include "config.h"

void Buttons::init()
{
    pinMode(CFG_BTN_START_WEB_SERVER, INPUT_PULLUP);
    pinMode(CFG_BTN_DISABLE_MOTORS, INPUT_PULLUP);
    Serial.println("Buttons initialised");
}

void Buttons::loop()
{
    if (digitalRead(CFG_BTN_START_WEB_SERVER) == LOW && !webServerBtnDown)
    {
        webServerBtnDown = true;
        Serial.println("Button to start web server pressed");
    }

    if (digitalRead(CFG_BTN_START_WEB_SERVER) == HIGH && webServerBtnDown)
    {
        webServerBtnDown = false;
        Serial.println("Button to start web server released");
    }

    if (digitalRead(CFG_BTN_DISABLE_MOTORS) == HIGH && disableMotorsBtnDown)
    {
        disableMotorsBtnDown = false;
        Serial.println("Button to disable motors released");
    }

    if (digitalRead(CFG_BTN_DISABLE_MOTORS) == LOW && !disableMotorsBtnDown)
    {
        disableMotorsBtnDown = true;
        Serial.println("Button to disable motors pressed");
    }
}