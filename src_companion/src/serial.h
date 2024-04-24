#pragma once

class QOctoSerial
{
public:
    QOctoSerial();
    void loop();

private:
    uint8_t serialBuffer[255];
};