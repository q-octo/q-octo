#include "serial.h"

void SerialTask::init() {
    queue = xQueueCreate(10, sizeof(SendSerialData));
}

void SerialTask::loop() {
    static SendSerialData data;
    if (xQueueReceive(queue, &data, 0) == pdTRUE) {
        memcpy(sendBuffer, data.data, data.size);
        switch (data.target) {
            case COMPANION:
                companionSerial.write(sendBuffer, data.size);
                break;
            case ONBOARD_COMPUTER:
                computerSerial.write(sendBuffer, data.size);
                break;
        }
    }
}