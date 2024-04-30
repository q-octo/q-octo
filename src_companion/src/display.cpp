
#include "display.h"

#define SET_PEN_RED() graphics.set_pen(244, 67, 54);
#define SET_PEN_GREEN() graphics.set_pen(76, 176, 80);
#define SET_PEN_ORANGE() graphics.set_pen(255, 152, 0);
#define SET_PEN_BLACK() graphics.set_pen(0, 0, 0);
#define SET_PEN_WHITE() graphics.set_pen(255, 255, 255);

void Display::loop()
{
  static unsigned long lastBlinkMillis = 0;
  static unsigned long lastRepaintMillis = 0;

  st7789.set_backlight(255);
  led.set_brightness(0); // Turn LED off.

  // Display is 240x135 pixels

  // Blink the LED every second to show that the data is live
  // TODO blink this red if the status is not OK.
  const uint32_t currentMillis = millis();
  if (currentMillis - lastBlinkMillis >= 1000)
  {
    lastBlinkMillis = currentMillis;
    blinkLED();
  }

  // Repaint the screen every 32ms (30fps)
  if (currentMillis - lastRepaintMillis >= 32)
  {
    lastRepaintMillis = currentMillis;
    repaintDisplay();
  }
}

void Display::blinkLED()
{
  led.set_rgb(76, 176, 80); // Green
  led.set_brightness(ledState ? 0 : 100);
  ledState = !ledState;
}

void Display::repaintDisplay()
{

  // An example of how to access state.
  //  state.batteries;
  
  // bitmap6, bitmap8, bitmap14_outline

  // Set white background
  SET_PEN_WHITE()
  graphics.clear();

  // TODO: Top Bar - Control Source and WIFI
  // Height 30px
    graphics.set_font("bitmap14_outline");
  SET_PEN_GREEN()
    Rect topBar(0, 0, 240, 30);
  graphics.rectangle(topBar);
    topBar.deflate(2);

    SET_PEN_BLACK()
    graphics.text("FL_CON", Point(topBar.x, topBar.y), topBar.w);
    graphics.text("WIFI: OFF", Point(topBar.x + 100, topBar.y), topBar.w);


  // TODO: Middle1 - Status Code in the center
  // Height 31px
    graphics.set_font("bitmap14_outline");
    // sans, add s
    SET_PEN_RED()
    Rect middle1(0, 30, 240, 31);
    graphics.rectangle(middle1);
    middle1.deflate(2);

    SET_PEN_BLACK()
    // Center the text
    // Text is 14px
    // Measure length of text first
    const char *status = "ALL SYSTEMS GO";
    int32_t textWidth = graphics.measure_text(status, 2, 1, false);
    graphics.text(status, Point(middle1.x + (middle1.w - textWidth) / 2, middle1.y), middle1.w);

  // TODO: Middle2 - battery bar
  // Height 20px
    SET_PEN_BLACK()
    Rect middle2(0, 61, 240, 20);
    graphics.rectangle(middle2);
    middle2.deflate(2);

    SET_PEN_GREEN()
    int padding = 2;
    graphics.rectangle(Rect(0 + padding, 61 + padding, (int)((240-padding) * 0.84), 20 - padding * 2));


  // TODO: Bottom1 - Voltage and number of batteries
    graphics.set_font("bitmap8");
    // Height 27px
    SET_PEN_GREEN()
    Rect bottom1(0, 81, 240, 27);
    graphics.rectangle(bottom1);
    bottom1.deflate(2);

    SET_PEN_BLACK()
    graphics.text("VOLTAGE: 22.1V", Point(bottom1.x, bottom1.y), bottom1.w);
    // Draw a battery icon with 2 rectangles

    graphics.rectangle(Rect(bottom1.x + 185, bottom1.y, 24, 18));
    graphics.rectangle(Rect(bottom1.x + 209, bottom1.y + 6, 8, 8));

    graphics.text("#4", Point(bottom1.x + 160, bottom1.y), bottom1.w);

  // TODO: Bottom2 - Current and number of battery percentage
  // Height 27px
    SET_PEN_RED()
    Rect bottom2(0, 108, 240, 27);
    graphics.rectangle(bottom2);
    bottom2.deflate(2);

    SET_PEN_BLACK()
    graphics.text("CURRENT: 14.5A", Point(bottom2.x, bottom2.y), bottom2.w);
    graphics.text("84%", Point(bottom2.x + 160, bottom2.y), bottom2.w);

    // now we've done our drawing let's update the screen
  st7789.update(&graphics);
}