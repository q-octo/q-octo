/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
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
    void init();
    void loop();
    void updateState(const RobotT& robot) { state = robot; } 

private:
    void paintStack(const std::vector<std::string> &items);
    std::string fmtFloat(const float& number, const int& precision = 2);
    void repaintDisplay();
    void paintPage1();
    void paintPage2();
    void paintPage3();
    void paintPage4();
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
    int current_page = 0;
    int max_pages = 4;
    void nextPage() { current_page = (current_page + 1) % max_pages; }
    void prevPage() { current_page = (current_page - 1 + max_pages) % max_pages; }

    RobotT state = RobotT();

    bool ledState = false;
 
};
