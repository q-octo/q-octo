
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

    // Check for button presses

    if (button_y.read())
    {
      nextPage();
    }
}

void Display::blinkLED()
{
  led.set_rgb(76, 176, 80); // Green
  led.set_brightness(ledState ? 0 : 100);
  ledState = !ledState;
}

void Display::paintStack(std::vector<std::string> items) {
    const int displayWidth = 240;
    const int displayHeight = 135;
    graphics.set_font("bitmap8");
    const int maxMessages = 7;
    int numMessages = std::min((int)items.size(), maxMessages);
    int boxHeight = displayHeight / numMessages;
    int paddingL = 2;

    SET_PEN_WHITE();
    graphics.rectangle(Rect(0, 0, displayWidth, displayHeight)); // Set whole background white

    SET_PEN_BLACK();
    for (int i = 0; i < numMessages; i++) {
        // Calculate the y position of the top of this message box
        int boxTop = i * boxHeight;

        // Draw dividing line (if not the first box)
        if (i > 0) {
            graphics.line(Point(0, boxTop), Point(displayWidth, boxTop));
        }

        // Calculate centering of the text within the box
        const char* text = items[i].c_str();
        int textWidth = graphics.measure_text(text, 2, 1, false);
        int textX = paddingL; // (displayWidth - textWidth) / 2;
        int textY = boxTop + (boxHeight - 14) / 2; // Assuming font height is 14px

        graphics.text(text, Point(textX, textY), displayWidth);
    }
}

void Display::paintPage1()
{
// An example of how to access state.
    //  state.batteries;

    // bitmap6, bitmap8, bitmap14_outline

    // For building strings
    std::ostringstream oss;

    // Set white background
    SET_PEN_WHITE()

    // Top Bar - Control Source and WIFI
    // Height 30px
    graphics.set_font("bitmap14_outline");
    SET_PEN_WHITE()
    Rect topBar(0, 0, 240, 30);
    graphics.rectangle(topBar);
    topBar.deflate(2);

    SET_PEN_BLACK()
    graphics.text("ONBCOM", Point(topBar.x, topBar.y), topBar.w);

    // TODO: Make this color conditional on wifi on/off
    graphics.text("WIFI:", Point(topBar.x + 110, topBar.y), topBar.w);

    int wifiwidth = graphics.measure_text("WIFI:", 2, 1, false);

    SET_PEN_GREEN();

    graphics.text("ON", Point(topBar.x + 110 + wifiwidth, topBar.y), topBar.w);


    // TODO: Middle1 - Status Code in the center
    // Height 31px
    graphics.set_font("bitmap14_outline");

    // TODO: Make this color conditional on status
    SET_PEN_GREEN()
    Rect middle1(0, 30, 240, 31);
    graphics.rectangle(middle1);
    middle1.deflate(2);

    SET_PEN_WHITE()
    // Center the text
    // Text is 14px
    // Measure length of text first
    const char *status = "ALL SYSTEMS GO";
    int32_t textWidth = graphics.measure_text(status, 2, 1, false);
    graphics.text(status, Point(middle1.x + (middle1.w - textWidth) / 2, middle1.y), middle1.w);

    // Middle2 - battery bar
    // Height 20px
    SET_PEN_BLACK()
    Rect middle2(0, 61, 240, 20);
    graphics.rectangle(middle2);
    middle2.deflate(2);

    SET_PEN_GREEN()
    int padding = 2;
    graphics.rectangle(Rect(0 + padding, 61 + padding, (int)((240-padding) * 0.84), 20 - padding * 2));


    // Bottom1 - Voltage and number of batteries
    graphics.set_font("bitmap8");
    // Height 27px
    SET_PEN_WHITE()
    Rect bottom1(0, 81, 240, 27);
    graphics.rectangle(bottom1);
    bottom1.deflate(2);

    SET_PEN_BLACK()
    // TODO: Make this color conditional on voltage
    graphics.text("VOLTAGE:", Point(bottom1.x, bottom1.y), bottom1.w);

    if (state.voltage < 11.5) {
        SET_PEN_RED()
    } else if (state.voltage < 12.5) {
        SET_PEN_ORANGE()
    } else {
        SET_PEN_GREEN()
    }

    // Formatted string with voltage
    oss.str("");
    oss << state.voltage << "V";

    graphics.text(oss.str(), Point(bottom1.x + 100, bottom1.y), bottom1.w);

    // Draw a battery icon with 2 rectangles
    SET_PEN_BLACK()
    graphics.rectangle(Rect(bottom1.x + 185, bottom1.y, 24, 18));
    graphics.rectangle(Rect(bottom1.x + 209, bottom1.y + 6, 8, 8));

    oss.str("");
    oss << "#" << state.batteries ;

    graphics.text(oss.str(), Point(bottom1.x + 160, bottom1.y), bottom1.w);

    // Bottom2 - Current and number of battery percentage
    // Height 27px
    SET_PEN_WHITE()
    Rect bottom2(0, 108, 240, 27);
    graphics.rectangle(bottom2);
    bottom2.deflate(2);

    SET_PEN_BLACK()
    // Make this color conditional on current
    graphics.text("CURRENT:", Point(bottom2.x, bottom2.y), bottom2.w);

    if (state.current < 0) {
        SET_PEN_RED()
    } else if (state.current < 1) {
        SET_PEN_ORANGE()
    } else {
        SET_PEN_GREEN()
    }

    oss.str("");
    oss << state.current << "A";
    graphics.text(oss.str(), Point(bottom2.x + 100, bottom2.y), bottom2.w);

    if (state.fuel < 0.1) {
        SET_PEN_RED()
    } else if (state.fuel < 0.2) {
        SET_PEN_ORANGE()
    } else {
        SET_PEN_GREEN()
    }

    oss.str("");
    oss << state.fuel * 100 << "%";

    graphics.text(oss.str(), Point(bottom2.x + 160, bottom2.y), bottom2.w);

    // now we've done our drawing let's update the screen
}

void Display::paintPage2() {
    // Set white background
    SET_PEN_WHITE()
    paintStack({"RSSI -77dB L:64% SNR 10",
                      "M_l: 22°C 20RAD/S 180°",
                      "M_r: 22°C 20RAD/S 180°",
                      "1-4: 1500 1500 1500 1500",
                        "5-8: 1500 1500 1500 1500",
                        "9-12: 1500 1500 1500 1500",
                        "13-16: 1500 1500 1500 1500",
                    });
}

void Display::paintPage3() {
    // Set white background
    SET_PEN_WHITE()
}


void Display::repaintDisplay()
{
    graphics.clear();
    switch (current_page)
    {
    case 0:
        paintPage1();
        break;
    case 1:
        paintPage2();
        break;
    case 2:
        paintPage3();
        break;
    }
  st7789.update(&graphics);
}