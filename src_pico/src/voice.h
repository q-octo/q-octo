#pragma once
#include <Arduino.h>
#include <SerialPIO.h>
#include "config.h"

class Voice
{
public:
    typedef enum : uint8_t
    {
        EASTER_EGG = 1,
        OK,
        LOW_BATTERY,
        TX_LOST,
        TX_RESTORED,
        MOTOR_ERROR,
        DISARMED,
        POWER_ON,
        NAVIGATE_VIA_ONBOARD_COMPUTER,
        NAVIGATE_VIA_FLIGHT_CONTROLLER,
        NAVIGATE_VIA_RADIO,
        DASHBOARD_ENABLED,
        DASHBOARD_DISABLED,
        FOLDING_WHEELS,
        STAND_CLEAR,
    } Announcement;
    static void init();
    static void announce(Announcement announcement);

private:
    static inline SerialPIO voiceSerial = SerialPIO(CFG_VOICE_UART_TX, CFG_VOICE_UART_RX);

    static void setVolume(unsigned char vol);
    static void playTrackDocumented(uint16_t track);
    static void playTrack(unsigned char track);
};