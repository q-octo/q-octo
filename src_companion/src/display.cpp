
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

  if (state == nullptr)
  {
    // Skip display update because we do not have anything to show yet.
    return;
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
  state->batteries();
  
  // bitmap6, bitmap8, bitmap14_outline
  graphics.set_font("bitmap8");

  // Set white background
  SET_PEN_WHITE()
  graphics.clear();

  int32_t motorBoxWidth = 160;

  // Left Motor
  SET_PEN_GREEN()
  Rect leftMotorRect(0, 0, motorBoxWidth, 18);
  // graphics.rectangle(leftMotorRect);
  leftMotorRect.deflate(2);
  SET_PEN_BLACK()
  // Format string based on motorLtemperature, motorLRPM, motorLposition

  std::ostringstream oss;
  oss << "L " << motorLtemperature << "°C " << motorLRPM << "RPM " << motorLposition << "°";
  graphics.text(oss.str(), Point(leftMotorRect.x, leftMotorRect.y),
                leftMotorRect.w);

  // Right Motor
  SET_PEN_RED()
  Rect rightMotorRect(0, 18, motorBoxWidth, 18);
  // graphics.rectangle(rightMotorRect);
  rightMotorRect.deflate(2);
  SET_PEN_BLACK()

  oss.str("");
  oss << "R " << motorRtemperature << "°C " << motorRRPM << "RPM " << motorRposition << "°";
  graphics.text(oss.str(), Point(rightMotorRect.x, rightMotorRect.y),
                rightMotorRect.w);

  // Vertical separator
  SET_PEN_BLACK()
  graphics.rectangle(Rect(motorBoxWidth, 0, 2, 36));

  // WiFi Status
  SET_PEN_ORANGE()
  Rect wifiRect(motorBoxWidth + 2, 0, 240 - motorBoxWidth, 36);
  // graphics.rectangle(wifiRect);
  wifiRect.deflate(2);
  if (button_x.read())
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
  graphics.text("FUEL   84%", Point(batteryRect1.x, batteryRect1.y),
                batteryRect1.w);

  // BATT
  SET_PEN_GREEN()
  Rect batteryRect2(0, 76, 100, 18);
  // graphics.rectangle(batteryRect2);
  batteryRect2.deflate(2);
  SET_PEN_BLACK()
  graphics.text("BATT 22.1V", Point(batteryRect2.x, batteryRect2.y),
                batteryRect2.w);

  // CURR
  SET_PEN_ORANGE()
  Rect batteryRect3(0, 94, 100, 18);
  // graphics.rectangle(batteryRect3);
  batteryRect3.deflate(2);
  SET_PEN_BLACK()
  graphics.text("CURR 14.5A", Point(batteryRect3.x, batteryRect3.y),
                batteryRect3.w);

  // Progress bars

  SET_PEN_GREEN()
  uint32_t maxWidth = 139; // 240 - 100 - 1(padding);
  Rect fuelBar(101, 58, maxWidth * batteryFuel, 18);
  fuelBar.deflate(2);
  graphics.rectangle(fuelBar);
  Rect battBar(101, 76, maxWidth * batteryVoltage, 18);
  battBar.deflate(2);
  graphics.rectangle(battBar);
  Rect currBar(101, 94, maxWidth * batteryCurrent, 18);
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
}