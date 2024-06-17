/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

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
  // HIGH = ON because a disconnected switch should be on by default
  if (digitalRead(CFG_BTN_START_WEB_SERVER) == HIGH && !webServerSwitchEnabled) {
    webServerSwitchEnabled = true;
    Serial.println("Button to start web server pressed");

    message = {
            .type = DataManager::BUTTON_DOWN,
            .as = {.physicalButton = DataManager::PhysicalButton::WEB_SERVER}
    };
    DataManager::receiveMessage(message);
  }

  if (digitalRead(CFG_BTN_START_WEB_SERVER) == LOW && webServerSwitchEnabled) {
    webServerSwitchEnabled = false;
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