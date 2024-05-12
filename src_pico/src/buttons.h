#pragma once

// We have two buttons, one to start the web dashboard, and one to disable motors.

class Buttons
{
public:
    static void init();
    static void loop();
private:
    static inline bool webServerSwitchEnabled = false;
    static inline bool disableMotorsBtnDown = false;
};