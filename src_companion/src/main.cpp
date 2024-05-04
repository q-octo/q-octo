#include <Arduino.h>

#include "display.h"
#include "web_server.h"
#include "serial.h"

#define WAIT_FOR_USB_SERIAL 1

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
}

void loop() {
  display.loop();
  webServer.loop();
  QOctoSerial::loop();
}

void onReceiveSerialMessage(const CompanionRxT &message) {
  Serial.println("We got a message");
  auto msg = message.message;
  switch (msg.type) {
    case CompanionRxUnion_NONE:
      break;
    case CompanionRxUnion_SetWebServerEnabled:
      if (msg.AsSetWebServerEnabled()->enabled) {
        webServer.start();
      } else {
        webServer.stop();
      }
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
    