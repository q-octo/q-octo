#include "buttons.h"
#include <Arduino.h>
#include "config.h"
#include "task_data_manager.h"

void Buttons::init() {
  pinMode(CFG_BTN_START_WEB_SERVER, INPUT_PULLUP);
  pinMode(CFG_BTN_DISABLE_MOTORS, INPUT_PULLUP);
  Serial.println("Buttons initialised");
}

void Buttons::loop() {
  static DataManager::Message message;
  if (digitalRead(CFG_BTN_START_WEB_SERVER) == LOW && !webServerBtnDown) {
    webServerBtnDown = true;
    Serial.println("Button to start web server pressed");

    message = {
            .type = DataManager::BUTTON_DOWN,
            .as = {.physicalButton = DataManager::PhysicalButton::WEB_SERVER}
    };
    DataManager::receiveMessage(message);
  }

  if (digitalRead(CFG_BTN_START_WEB_SERVER) == HIGH && webServerBtnDown) {
    webServerBtnDown = false;
    Serial.println("Button to start web server released");

    message = {
            .type = DataManager::BUTTON_UP,
            .as = {.physicalButton = DataManager::PhysicalButton::WEB_SERVER}
    };
    DataManager::receiveMessage(message);
  }

  if (digitalRead(CFG_BTN_DISABLE_MOTORS) == LOW && !disableMotorsBtnDown) {
    disableMotorsBtnDown = true;
    Serial.println("Button to disable motors pressed");

    message = {
            .type = DataManager::BUTTON_DOWN,
            .as = {.physicalButton = DataManager::PhysicalButton::MOTORS}
    };
    DataManager::receiveMessage(message);
  }

  if (digitalRead(CFG_BTN_DISABLE_MOTORS) == HIGH && disableMotorsBtnDown) {
    disableMotorsBtnDown = false;
    Serial.println("Button to disable motors released");

    message = {
            .type = DataManager::BUTTON_UP,
            .as = {.physicalButton = DataManager::PhysicalButton::MOTORS}
    };
    DataManager::receiveMessage(message);
  }


}