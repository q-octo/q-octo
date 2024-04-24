#pragma once

// May need to end up in main.cpp to be enabled??
// Otherwise, it can go in the .cpp file.
#include <WiFi.h>
#include "Arduino.h"

namespace WSWebServer
{

    void init();
    void start();
    void stop();
    void loop();
typedef enum {
    ENABLE,
    DISABLE,
    STATE_UPDATE,
} MessageType;

typedef struct {
    MessageType type;
    union {
        // TaskMessage::State state;
    } as;
} Message;

}


extern bool webServerEnabled;
