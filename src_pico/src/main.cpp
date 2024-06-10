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