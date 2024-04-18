#include <WiFi.h>
#include "Arduino.h"
#include <FreeRTOS.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <LEAmDNS.h>
#include <semphr.h>

SemaphoreHandle_t semaphore;
// QueueHandle_t wifiQueue;

WebServer debugWebServer(80);
WebSocketsServer debugWebSocket(81);

void setup()
{
  // wifiQueue = xQueueCreate(10, sizeof(int));
  Serial.begin(115200);
  semaphore = xSemaphoreCreateBinary();
  WiFi.mode(WIFI_AP);
  IPAddress accessPointIP(192, 168, 4, 1);
  WiFi.softAPConfig(accessPointIP, accessPointIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Pico W WiFi Crash Demo");
  delay(5000);
  debugWebServer.on("/", []()
                    {
                // Serve hello world in html
                String html_code = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>WebSocket Example</title><script>document.addEventListener('DOMContentLoaded', function () { const randomNumber = Math.floor(Math.random() * 10000); const socket = new WebSocket('ws://picow.local:81'); socket.addEventListener('open', function (event) { console.log('Connected to the WebSocket server'); setInterval(() => { socket.send('hello from ' + randomNumber); console.log('Message sent: hello from ' + randomNumber); }, 1000); }); socket.addEventListener('message', function (event) { console.log('Message from server ', event.data); }); socket.addEventListener('error', function (event) { console.error('WebSocket error:', event); }); socket.addEventListener('close', function (event) { console.log('WebSocket connection closed'); }); });</script></head><body><h1>WebSocket Communication</h1><p>Check the console for messages sent and received.</p></body></html>";

                debugWebServer.send(200, "text/html", html_code); 
                });
  debugWebServer.begin();
  Serial.println("Web server started");
  debugWebSocket.begin();
  debugWebSocket.onEvent([](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
                         {
                      switch (type)
                      {
                      case WStype_DISCONNECTED:
                        Serial.printf("[%u] Disconnected!\n", num);
                        break;
                      case WStype_CONNECTED:
                      {
                        IPAddress ip = debugWebSocket.remoteIP(num);
                        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                      }
                      break;
                      case WStype_TEXT:
                        Serial.printf("[%u] get Text: %s\n", num, payload);
                        break;
                      case WStype_BIN:
                        Serial.printf("[%u] get binary length: %u\n", num, length);
                        break;
                      } });
  Serial.println("Web socket server started");

  if (!MDNS.begin("picow"))
  {
    Serial.println("Error setting up MDNS responder!");
  }
  else
  {
    Serial.println("MDNS responder started");
    // Add service to mDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
  }
}

void loop()
{
  int x;
  // This line causes the crash
  // BaseType_t xStatus = xQueueReceive(wifiQueue, &x, pdMS_TO_TICKS(1));

  static int i;
  delay(1000);
  Serial.printf("--loop %d\n", ++i);
  delay(10);
  Serial.printf("Take semaphore\n");
  xSemaphoreTake(semaphore, pdMS_TO_TICKS(10));

  debugWebServer.handleClient();
  debugWebSocket.loop();
  MDNS.update();
}