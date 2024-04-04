#pragma once

// May need to end up in main.cpp to be enabled??
// Otherwise, it c an go in the .cpp file.
#include <WiFi.h>

namespace WSWebServer
{

    void start();
    void stop();
    void loop();
}