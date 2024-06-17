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
#include <Arduino.h>
#include "config.h"
#include "storage.h"
#include "serial.h"
#include "task_motors.h"
#include "task_rc.h"
#include "task_watchdog.h"
#include "companion.h"
#include "computer.h"
#include "task_can.h"
#include "task_power_monitor.h"
#include "buttons.h"
#include "task_data_manager.h"
#include "enable_wifi.h"
#include "flight_controller.h"


void setup() {
  Serial.begin(115200);
  if (CFG_WAIT_FOR_USB_SERIAL) {
    // Wait for serial connection to be established
    while (!Serial);
  }
  delay(2000); // Wait for motors to receive power.
  Serial.println("Live on core 0");

  Storage::init(); // Intentionally initialised first
  DataManager::init();
  TaskCAN::init();
  TaskRC::init();
  TaskPowerMonitor::init();
  TaskControlMotors::init();
  Companion::init();
  Computer::init();
  Watchdog::init();
  Buttons::init();
  EnableWifi::init();
  FlightController::init();
}

void loop() {
  TaskCAN::loop();
  TaskRC::loop();
  TaskControlMotors::loop();
  Companion::loop();
  Computer::loop();
  Buttons::loop();

  Watchdog::taskCompleted(Watchdog::Task::FAST_LOOP);
  Watchdog::loop();
}

/*
void setup1() {
  SerialTask::init();
}

void loop1() { 
  SerialTask::loop();
}
*/