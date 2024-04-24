#include <Arduino.h>
#include <WiFi.h>

#include "display.h"

#define PIN_UART_TX 0
#define PIN_UART_RX 1

void loopWebServer();
void loopSerial();

void setup()
{
  Serial.begin(115200);
  // Wait for serial connection to be established
  while (!Serial)
    ;

  delay(1000); // Wait for a second
  Serial.println("Live on COMPANION PICO core 0");

  WSWebServer::init();
}

void loop()
{
  loopWebServer();
  loopDisplay();
  loopSerial();
}

void loopWebServer() {}
void loopSerial() {}