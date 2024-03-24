
#include <Arduino.h> // Always include this first
#include "FreeRTOS.h"
#include "task.h"
// Pico Display libraries
#include "pico_display.hpp"
#include "st7789.hpp"
#include "pico_graphics.hpp"
#include "rgbled.hpp"
#include "button.hpp"


// Explicitly include necessary classes from the pimoroni namespace
using pimoroni::ST7789;
using pimoroni::PicoDisplay;
using pimoroni::PicoGraphics_PenRGB332;
using pimoroni::ROTATE_0;
using pimoroni::RGBLED;
using pimoroni::Button;
using pimoroni::Rect;
using pimoroni::Point;
using pimoroni::get_spi_pins;
using pimoroni::BG_SPI_FRONT;

// Display driver
ST7789 st7789(PicoDisplay::WIDTH, PicoDisplay::HEIGHT, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT));

// Graphics library - in RGB332 mode you get 256 colours and optional dithering for ~32K RAM.
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);

// RGB LED
RGBLED led(PicoDisplay::LED_R, PicoDisplay::LED_G, PicoDisplay::LED_B);

// And each button
Button button_a(PicoDisplay::A);
Button button_b(PicoDisplay::B);
Button button_x(PicoDisplay::X);
Button button_y(PicoDisplay::Y);

#define SET_PEN_RED() graphics.set_pen(244, 67, 54);
#define SET_PEN_GREEN() graphics.set_pen(76, 176, 80);
#define SET_PEN_ORANGE() graphics.set_pen(255, 152, 0);
#define SET_PEN_BLACK() graphics.set_pen(0, 0, 0);
#define SET_PEN_WHITE() graphics.set_pen(255, 255, 255);

unsigned long lastBlinkMillis = 0;
bool ledState = false;

void taskDisplay(void *pvParameters)
{
    (void)pvParameters; // To avoid warnings

    st7789.set_backlight(100);
    led.set_brightness(0); // Turn LED off.

    // Display is 240x135 pixels
    for (;;)
    {
        // bitmap6, bitmap8, bitmap14_outline
        graphics.set_font("bitmap8");

        // set the colour of the pen
        // parameters are red, green, blue all between 0 and 255
        SET_PEN_WHITE()
        // fill the screen with the current pen colour
        graphics.clear();

        // Blink the LED every second to show that the data is live
        unsigned long currentMillis = millis();
        if (currentMillis - lastBlinkMillis >= 1000)
        {
            lastBlinkMillis = currentMillis;
            led.set_rgb(76, 176, 80);
            led.set_brightness(ledState ? 0 : 100);
            ledState = !ledState;
        }

        int32_t motorBoxWidth = 160;

        // Left Motor
        SET_PEN_GREEN()
        Rect leftMotorRect(0, 0, motorBoxWidth, 18);
        // graphics.rectangle(leftMotorRect);
        leftMotorRect.deflate(2);
        SET_PEN_BLACK()
        graphics.text("22°C 30RPM 180°", Point(leftMotorRect.x, leftMotorRect.y), leftMotorRect.w);

        // Right Motor
        SET_PEN_RED()
        Rect rightMotorRect(0, 18, motorBoxWidth, 18);
        // graphics.rectangle(rightMotorRect);
        rightMotorRect.deflate(2);
        SET_PEN_BLACK()
        graphics.text("22°C 30RPM 180°", Point(rightMotorRect.x, rightMotorRect.y), rightMotorRect.w);

        // Vertical separator
        SET_PEN_BLACK()
        graphics.rectangle(Rect(motorBoxWidth, 0, 2, 36));

        // WiFi Status
        SET_PEN_ORANGE()
        Rect wifiRect(motorBoxWidth + 2, 0, 240 - motorBoxWidth, 36);
        // graphics.rectangle(wifiRect);
        wifiRect.deflate(2);
        if (button_x.raw())
        {
            SET_PEN_RED()
            graphics.text("WiFi  ON", Point(wifiRect.x, wifiRect.y), wifiRect.w);
        }
        else
        {
            SET_PEN_BLACK()
            graphics.text("WiFi OFF", Point(wifiRect.x, wifiRect.y), wifiRect.w);
        }

        // Horizontal separator
        SET_PEN_BLACK()
        graphics.rectangle(Rect(0, 36, 240, 1));

        // Batteries, SRC, Status
        SET_PEN_GREEN();
        Rect brsPlaceholder(0, 37, 240, 20);
        // graphics.rectangle(brsPlaceholder);

        // Horizontal separator
        SET_PEN_BLACK()
        graphics.rectangle(Rect(0, 57, 240, 1));

        // Battery section

        // Progress bar background
        SET_PEN_BLACK()
        // graphics.rec tangle(Rect(100, 58, 240 - 100, 135 - 58));

        // Fuel
        SET_PEN_RED()
        Rect batteryRect1(0, 58, 100, 18);
        // graphics.rectangle(batteryRect1);
        batteryRect1.deflate(2);
        SET_PEN_BLACK()
        graphics.text("FUEL   84%", Point(batteryRect1.x, batteryRect1.y), batteryRect1.w);

        // BATT
        SET_PEN_GREEN()
        Rect batteryRect2(0, 76, 100, 18);
        // graphics.rectangle(batteryRect2);
        batteryRect2.deflate(2);
        SET_PEN_BLACK()
        graphics.text("BATT 22.1V", Point(batteryRect2.x, batteryRect2.y), batteryRect2.w);

        // CURR
        SET_PEN_ORANGE()
        Rect batteryRect3(0, 94, 100, 18);
        // graphics.rectangle(batteryRect3);
        batteryRect3.deflate(2);
        SET_PEN_BLACK()
        graphics.text("CURR 14.5A", Point(batteryRect3.x, batteryRect3.y), batteryRect3.w);

        // Progres bars
        float fuel = 0.84;
        float batt = 0.67;
        float curr = 0.72;
        SET_PEN_GREEN()
        uint32_t maxWidth = 139; // 240 - 100 - 1(padding);
        Rect fuelBar(101, 58, maxWidth * fuel, 18);
        fuelBar.deflate(2);
        graphics.rectangle(fuelBar);
        Rect battBar(101, 76, maxWidth * batt, 18);
        battBar.deflate(2);
        graphics.rectangle(battBar);
        Rect currBar(101, 94, maxWidth * curr, 18);
        currBar.deflate(2);
        graphics.rectangle(currBar);

        // Make the text smaller
        graphics.set_font("bitmap6");

        // CRSF Status
        SET_PEN_RED()
        Rect rcRectL(1, 112, 100, 135 - 112);
        // graphics.rectangle(rcRectL);
        SET_PEN_BLACK()
        graphics.text("RSSI 77dB", Point(rcRectL.x, rcRectL.y), rcRectL.w);
        graphics.text("LINK  64%", Point(rcRectL.x, rcRectL.y + 11), rcRectL.w);

        // CRSF Progress Bars
        SET_PEN_ORANGE()
        Rect rssiBar(101, 112 + 1, maxWidth * 0.77, 11);
        rssiBar.deflate(1);
        graphics.rectangle(rssiBar);
        Rect linkBar(101, 112 + 11 + 1, maxWidth * 0.64, 11);
        linkBar.deflate(1);
        graphics.rectangle(linkBar);

        // now we've done our drawing let's update the screen
        st7789.update(&graphics);
        vTaskDelay(pdMS_TO_TICKS(32));
    }
}

