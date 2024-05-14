#include <Arduino.h>

#include "display.h"
#include "web_server.h"
#include "serial.h"

#define WAIT_FOR_USB_SERIAL 0
#define DEBUG_ENABLE_WIFI_ON_LAUNCH 0

void toggleAButton();

void toggleBButton();

void toggleXButton();

void onReceiveSerialMessage(const CompanionRxT &message);

// Callbacks can be added to these constructors if necessary.
Display display(toggleAButton, toggleBButton, toggleXButton);
QOctoWebServer webServer;

void setup() {
  Serial.begin(115200);
#if WAIT_FOR_USB_SERIAL
  while (!Serial) {
  }
#endif

  delay(1000); // Wait for a second
  Serial.println("Live on COMPANION PICO core 0");
  QOctoSerial::init(onReceiveSerialMessage);
  display.init();
#if DEBUG_ENABLE_WIFI_ON_LAUNCH
  webServer.start();
#endif
  // Start the watchdog timer
  rp2040.wdt_begin(15'000);
}

void loop() {
  display.loop();
  webServer.loop();
  QOctoSerial::loop();
  
  // Reset the watchdog timer
  rp2040.wdt_reset();
}

void onReceiveSerialMessage(const CompanionRxT &message) {
  Serial.println("We got a message");
  auto msg = message.message;
  switch (msg.type) {
    case CompanionRxUnion_NONE:
      break;
    case CompanionRxUnion_Robot:
      display.updateState(*msg.AsRobot());
      webServer.updateState(*msg.AsRobot());
      break;
  }

}

void toggleAButton() {
  Serial.println("A button pressed sending message to serial port");
  QOctoSerial::sendButtonPressed(fbs::Button::Button_A);
}

void toggleBButton() {
  Serial.println("B button pressed sending message to serial port");
  QOctoSerial::sendButtonPressed(fbs::Button::Button_B);
}

void toggleXButton() {
    Serial.println("X button pressed sending message to serial port");
  QOctoSerial::sendButtonPressed(fbs::Button::Button_X);
}
    