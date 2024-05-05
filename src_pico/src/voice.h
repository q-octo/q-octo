#pragma once

class Voice
{
public:
    typedef enum
    {
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
};