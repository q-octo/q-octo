#include <Arduino.h>

#include "display.h"
#include "web_server.h"
#include "serial.h"

#define WAIT_FOR_USB_SERIAL 1

void toggleAButton();
void toggleBButton();
void toggleXButton();

// Callbacks can be added to these constructors if necessary.
Display display(toggleAButton, toggleBButton, toggleXButton);
QOctoWebServer webServer;
QOctoSerial serial;

void setup()
{
  Serial.begin(115200);
#if WAIT_FOR_USB_SERIAL
  while (!Serial)
  {
  }
#endif

  delay(1000); // Wait for a second
  Serial.println("Live on COMPANION PICO core 0");
  QOctoSerial::init();
}

void loop()
{
  display.loop();
  webServer.loop();
  serial.loop();
}

void toggleAButton()
{
  Serial.println("A button pressed");
}

void toggleBButton()
{
  Serial.println("B button pressed");
}

void toggleXButton()
{
  Serial.println("X button pressed");
}
