#pragma once

#include "Arduino.h"

class Watchdog {
public:
  typedef enum {
    FAST_LOOP,
    numValues
  } Task;

  static void init();

  static void taskCompleted(Task task);

  static void loop();

private:
  static inline uint32_t lastMemoryLogMs = 0;
  static inline uint32_t lastHealthCheckMs = 0;
  static inline long tasks[Task::numValues] = {0};
};
