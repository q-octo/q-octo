#include "serial.h"
#include <flatbuffer_serial_parser.h>

void SerialTask::init()
{
    queue = xQueueCreate(10, sizeof(SendSerialData));
    computerSerial.begin(115200);
}

void SerialTask::loop()
{
    static SendSerialData data;
    if (xQueueReceive(queue, &data, 0) == pdTRUE)
    {
        memcpy(sendBuffer, data.data, data.size);
        switch (data.target)
        {
        case COMPANION:
        { 
            companionSerial.write(FlatbufferSerialParser::START_BYTE);
            if (companionSerial.write(sendBuffer, data.size)) {
                Serial.println("SENT TO COMPANION");
            } else  {
                // TODO we're hitting this branch :()
                Serial.println("FAILED TO SEND TO COMPANION");
            }
            break;
        }
        case ONBOARD_COMPUTER:
            computerSerial.write(sendBuffer, data.size);
            break;
        }
    }
}