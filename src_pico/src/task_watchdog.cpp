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
#include "config.h"
#include "task_watchdog.h"

#define WATCHDOG_TIMEOUT 250
#define LOG_MEMORY_FREQUENCY 3000
#define HEALTH_CHECK_FREQUENCY 150

void Watchdog::init() {
  if (!CFG_ENABLE_WATCHDOG) {
    return;
  }
  rp2040.wdt_begin(WATCHDOG_TIMEOUT);
  Serial.println("WATCHDOG ENABLED");
}

void Watchdog::taskCompleted(Watchdog::Task task) {
  if (!CFG_ENABLE_WATCHDOG) {
    return;
  }
  tasks[task] += 1;
}

void Watchdog::loop() {
  if (!CFG_ENABLE_WATCHDOG) {
    return;
  }
  const uint32_t currentMillis = millis();
  if (currentMillis - lastHealthCheckMs > HEALTH_CHECK_FREQUENCY) {
    lastHealthCheckMs = currentMillis;
    for (uint8_t i = 0; i < Task::numValues; i++) {
      if (tasks[i] == 0) {
        Serial.printf("[FATAL] Task %d is not operational\n", i);
        delay(WATCHDOG_TIMEOUT);
      }
    }
    // All tasks are operational
    rp2040.wdt_reset();
    memset(tasks, 0, sizeof(tasks));
  }

  if (currentMillis - lastMemoryLogMs > LOG_MEMORY_FREQUENCY) {
    lastMemoryLogMs = currentMillis;
    Serial.printf("free heap: %d\n", rp2040.getFreeHeap());
  }
}
