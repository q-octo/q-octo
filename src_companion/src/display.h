#pragma once

#include <Arduino.h>
#include "button.hpp"
#include "pico_display.hpp"
#include "pico_graphics.hpp"
#include "rgbled.hpp"
#include "st7789.hpp"
#include "robot_state_generated.h"
#include "config.h"
#include <sstream>

using namespace fbs;

// Explicitly include necessary classes from the pimoroni namespace
using pimoroni::BG_SPI_FRONT;
using pimoroni::Button;
using pimoroni::get_spi_pins;
using pimoroni::PicoDisplay;
using pimoroni::PicoGraphics_PenRGB332;
using pimoroni::Point;
using pimoroni::Rect;
using pimoroni::RGBLED;
using pimoroni::ROTATE_0;
using pimoroni::ST7789;

// typedef callback for a button press
typedef void (*ButtonCallback)(void);

class Display
{
public:
    Display(
        ButtonCallback button_a_callback,
        ButtonCallback button_b_callback,
        ButtonCallback button_x_callback) : st7789(PicoDisplay::WIDTH, PicoDisplay::HEIGHT, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT)),
                                            graphics(st7789.width, st7789.height, nullptr),
                                            led(PicoDisplay::LED_R, PicoDisplay::LED_G, PicoDisplay::LED_B),
                                            button_a(PicoDisplay::A),
                                            button_b(PicoDisplay::B),
                                            button_x(PicoDisplay::X),
                                            button_y(PicoDisplay::Y),
                                            button_a_callback(button_a_callback),
                                            button_b_callback(button_b_callback),
                                            button_x_callback(button_x_callback)

    {
    }
    void loop();
    void updateState(const RobotT& robot) { state = robot; }

private:
    void repaintDisplay();
    void blinkLED();

    ST7789 st7789;
    PicoGraphics_PenRGB332 graphics;
    RGBLED led;
    // Spare button
    Button button_a;
    // Spare button
    Button button_b;
    // Wi-Fi button
    Button button_x;
    // Toggle screen button
    Button button_y;
    ButtonCallback button_a_callback;
    ButtonCallback button_b_callback;
    ButtonCallback button_x_callback;

    RobotT state = RobotT();

    // Internal state
    // TODO phase this out in favor of the Robot object
    bool ledState = false;
    float batteryFuel = 0.0; // 0.0 to 1.0
    float batteryVoltage = 0.0;
    float batteryCurrent = 0.0;
    uint16_t motorLtemperature = 0;
    float motorLRPM = 0.0;
    float motorLposition = 0.0;
    uint16_t motorRtemperature = 0;
    float motorRRPM = 0.0;
    float motorRposition = 0.0;
};
