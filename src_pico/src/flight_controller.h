#pragma once

#include "crsf.h"   
#include <SerialPIO.h>
#include "config.h"

class FlightController
{
public:
    static void init();
    static void loop();
private:
    static inline crsf_instance flConCrsfIns;
    static inline SerialPIO fcSerial = SerialPIO(SerialPIO::NOPIN, CFG_FLIGHT_CONTROLLER_UART_RX, 1024);
    static inline uint8_t flConSerialBuffer[128] = {0};
    static inline uint8_t crsfFrameIndex = 0;
    static inline uint8_t crsfFrameLength = 0;
    static inline uint8_t crsfCrcIndex = 0;


    static void onBatteryUpdate(const crsf_payload_battery_sensor_t battery);
};