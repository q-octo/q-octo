
#include <Arduino.h>
#include "web_server.h"
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <LEAmDNS.h>
#include "packed_fs.h"

/*

We need to establish a format for sending messages across the web socket
connection.

First action will likely be the server sending all current configuration info.
I imagine that a key-value system will work for all the properties to be
managed, this way we can add new configurations with minimal changes everywhere.
*/

IPAddress apIP(192, 168, 4, 1);
WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

bool webServerIsRunning = false;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void handleRoot();

void WSWebServer::start()
{
    if (webServerIsRunning)
    {
        return;
    }
    Serial.println("Starting web server");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Q-Octo (rover.local)");
    delayMicroseconds(500 * 1000); // Without delay the IP might be blank (according to an example)
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP() + " (rover.local)");

    webServer.on("/", handleRoot);
    // webServer.onNotFound(handleRoot);
    webServer.begin();
    Serial.println("HTTP server started");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("Web socket server started");

    // Setup MDNS responder (creates rover.local domain within the AP)
    if (!MDNS.begin("rover"))
    {
        Serial.println("Error setting up MDNS responder!");
    }
    else
    {
        Serial.println("mDNS responder started");
        // Add service to MDNS-SD
        MDNS.addService("http", "tcp", 80);
        MDNS.addService("ws", "tcp", 81);
    }

    webServerIsRunning = true;
}

void WSWebServer::stop()
{
    webSocket.close();
    webServer.close();
    MDNS.close();
    WiFi.softAPdisconnect();

    webServerIsRunning = false;
}

void WSWebServer::loop()
{
    webServer.handleClient();
    webSocket.loop();
    MDNS.update();
}

void handleRoot()
{
    digitalWrite(LED_BUILTIN, 1);
    size_t htmlSize;
    const char* html = mg_unpack("/index.html", &htmlSize, nullptr);
    webServer.send(200, "text/html", html, htmlSize);
    digitalWrite(LED_BUILTIN, 0);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);

        // send message to client
        // webSocket.sendTXT(num, "message here");

        // send data to all connected clients
        // webSocket.broadcastTXT("message here");
        break;
    case WStype_BIN:
        Serial.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);

        // send message to client
        // webSocket.sendBIN(num, payload, length);
        break;
    }
}
