#pragma once

class QOctoSerial
{
public:
    static void init();
    void loop();

private:
    uint8_t serialBuffer[255];
};