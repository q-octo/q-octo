#pragma once

class QOctoSerial
{
public:
    QOctoSerial();
    void init();
    void loop();

private:
    uint8_t serialBuffer[255];
};