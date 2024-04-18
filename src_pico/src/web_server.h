#pragma once

// May need to end up in main.cpp to be enabled??
// Otherwise, it can go in the .cpp file.
#include <WiFi.h>
#include "Arduino.h"
// #include <FreeRTOS.h>
// #include <queue.h>
// #include <task.h>

namespace WSWebServer
{

    void init();
    void start();
    void stop();
    void loop();
typedef enum {
    ENABLE,
    DISABLE,
} MessageType;

typedef struct {
    MessageType type;
    union {} as;
} Message;

}

// extern QueueHandle_t webServerQueue;