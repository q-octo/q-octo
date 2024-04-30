
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
  if (currentMillis - lastRepaintMillis >= 64)
  {
    lastRepaintMillis = currentMillis;
    repaintDisplay();
  }

    // Check for button presses

    if (button_y.read())
    {
      nextPage();
      Serial.println("Button Y pressed");
    }
}

void Display::blinkLED()
{
  led.set_rgb(76, 176, 80); // Green
  led.set_brightness(ledState ? 0 : 100);
  ledState = !ledState;
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

    oss.str("");
    oss << EnumNamesControlSource()[state.control_source];
    SET_PEN_BLACK()
    graphics.text(oss.str(), Point(topBar.x, topBar.y), topBar.w);

    // TODO: Make this color conditional on wifi on/off
    graphics.text("WIFI:", Point(topBar.x + 110, topBar.y), topBar.w);
    int wifiwidth = graphics.measure_text("WIFI:", 2, 1, false);
    SET_PEN_GREEN();
    graphics.text("ON", Point(topBar.x + 110 + wifiwidth, topBar.y), topBar.w);


    // Middle1 - Status Code in the center
    // Height 31px
    graphics.set_font("bitmap14_outline");

    // Center the text
    // Text is 14px
    // Measure length of text first
    const char *status;

    status = EnumNamesStatus()[state.status];

    switch (state.status) {
        case Status_OK:
            SET_PEN_GREEN();
            break;
        case Status_INIT:
            SET_PEN_ORANGE()
            break;
        case Status_NOTX:
            SET_PEN_RED()
            break;
        case Status_BAT:
            SET_PEN_RED()
            break;
        case Status_TEMP:
            SET_PEN_ORANGE()
            break;
        case Status_OFF:
            SET_PEN_RED()
            break;
    }

    Rect middle1(0, 30, 240, 31);
    graphics.rectangle(middle1);
    middle1.deflate(2);

    int32_t textWidth = graphics.measure_text(status, 2, 1, false);
    SET_PEN_WHITE()
    graphics.text(status, Point(middle1.x + (middle1.w - textWidth) / 2, middle1.y), middle1.w);

    // Middle2 - battery bar
    // Height 20px
    SET_PEN_BLACK()
    Rect middle2(0, 61, 240, 20);
    graphics.rectangle(middle2);
    middle2.deflate(2);

    if (state.fuel < 0.1) {
        SET_PEN_RED()
    } else if (state.fuel < 0.2) {
        SET_PEN_ORANGE()
    } else {
        SET_PEN_GREEN()
    }

    int padding = 2;
    graphics.rectangle(Rect(0 + padding, 61 + padding, (int)((240-padding) * state.fuel), 20 - padding * 2));


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
}

void Display::paintPage2() {
    // Set white background
    SET_PEN_WHITE()
    graphics.clear();

    // Line1 - RSSI and Link Quality
    SET_PEN_BLACK()

    int xPosition = 10;
    int yPosition = 0;

    // Line1 - RSSI and Link Quality
    float rssi = state.rssi_threshold;
    float link = state.link_quality_threshold;

    graphics.text("RSSI:", Point(xPosition, yPosition), 220);
    xPosition += 50;
    graphics.text(std::to_string((int)rssi), Point(xPosition, yPosition), 220);
    xPosition += 50;
    graphics.text("L:", Point(xPosition, yPosition), 220);
    xPosition += 20;
    graphics.text(std::to_string((int)link), Point(xPosition, yPosition), 220);

    // Line2 - Motor1
    float temp1 = state.motors->motor1->temperature;
    float rps1 = state.motors->motor1->rps;
    float angle1 = state.motors->motor1->angle;

    xPosition = 10;
    yPosition = 25;

    graphics.text("M_l:", Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text(std::to_string((int)temp1), Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text("RPS:", Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text(std::to_string((int)rps1), Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text("A:", Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text(std::to_string((int)angle1), Point(xPosition, yPosition), 220);

    // Line3 - Motor2
    float temp2 = state.motors->motor2->temperature;
    float rps2 = state.motors->motor2->rps;
    float angle2 = state.motors->motor2->angle;

    xPosition = 10;
    yPosition = 50;

graphics.text("M_r:", Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text(std::to_string((int)temp2), Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text("RPS:", Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text(std::to_string((int)rps2), Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text("A:", Point(xPosition, yPosition), 220);
    xPosition += 40;
    graphics.text(std::to_string((int)angle2), Point(xPosition, yPosition), 220);

//    // Lines 4-7 - Channels 1-16
//    graphics.text("1-4: 1500 1500 1500 1500", Point(10, yPosition + 25), 220);
//    //graphics.line(Point(0, 105), Point(240, 105));  // Draw line
//
//    graphics.text("5-8: 1500 1500 1500 1500", Point(10, yPosition + 25), 220);
//    //graphics.line(Point(0, 130), Point(240, 130));  // Draw line
//
//    graphics.text("9-12: 1500 1500 1500 1500", Point(10, yPosition + 25), 220);
//    //graphics.line(Point(0, 155), Point(240, 155));  // Draw line
//
//    graphics.text("13-16: 1500 1500 1500 1500", Point(10, yPosition + 25), 220);
    //graphics.line(Point(0, 180), Point(240, 180));  // Draw line
    Serial.println("Painted page 2");
}

void Display::paintPage3() {
    // Set white background
    SET_PEN_WHITE()
    graphics.clear();

    // Prepare to display messages
    SET_PEN_BLACK()

    // Each message is accessed individually from the struct
    std::string message;
    int yPos = 10; // Start position for the first message

    message = "Msg1: " + std::string(state.display_messages->message1);
    graphics.text(message, Point(10, yPos), 220);
    yPos += 25;
    graphics.line(Point(0, yPos - 5), Point(240, yPos - 5));  // Draw line

    message = "Msg2: " + std::string(state.display_messages->message2);
    graphics.text(message, Point(10, yPos), 220);
    yPos += 25;
    graphics.line(Point(0, yPos - 5), Point(240, yPos - 5));  // Draw line

    message = "Msg3: " + std::string(state.display_messages->message3);
    graphics.text(message, Point(10, yPos), 220);
    yPos += 25;
    graphics.line(Point(0, yPos - 5), Point(240, yPos - 5));  // Draw line

    message = "Msg4: " + std::string(state.display_messages->message4);
    graphics.text(message, Point(10, yPos), 220);
    yPos += 25;
    graphics.line(Point(0, yPos - 5), Point(240, yPos - 5));  // Draw line

    message = "Msg5: " + std::string(state.display_messages->message5);
    graphics.text(message, Point(10, yPos), 220);
    yPos += 25;
    graphics.line(Point(0, yPos - 5), Point(240, yPos - 5));  // Draw line

    message = "Msg6: " + std::string(state.display_messages->message6);
    graphics.text(message, Point(10, yPos), 220);
    yPos += 25;
    graphics.line(Point(0, yPos - 5), Point(240, yPos - 5));  // Draw line

    message = "Msg7: " + std::string(state.display_messages->message7);
    graphics.text(message, Point(10, yPos), 220);
    yPos += 25;
    // Optional last line if you want a separator after the last message
    graphics.line(Point(0, yPos - 5), Point(240, yPos - 5));  // Draw line
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
    }
  st7789.update(&graphics);
}