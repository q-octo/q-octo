
#include <Arduino.h>
#include "web_server.h"
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <LEAmDNS.h>

/*
The web server is off by default.
When instructed, it will be started.
It can be stopped.
Anyway, it will establish a web socket connection and from there we can send
commands back and fourth as we wish.
Think back to the distributed systems & networks coursework, it's like this.
Create a list of instructions and the shape of their associated payload.

First action will likely be the server sending all current configuration info.
I imagine that a key-value system will work for all the properties to be
managed, this way we can add new configurations with minimal changes everywhere.


*/

int status = WL_IDLE_STATUS; // the WiFi server status
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

bool webServerIsRunning = false;
bool webServerInitialised = false;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

void initWebServer()
{
    if (webServerInitialised)
    {
        return;
    }
    webServerInitialised = true;
}

void handleRoot()
{
    digitalWrite(LED_BUILTIN, 1);
    webServer.send(200, "text/html", "<html><head><script>var connection = new WebSocket('ws://rover.local:81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};function sendRGB() {  var r = parseInt(document.getElementById('r').value).toString(16);  var g = parseInt(document.getElementById('g').value).toString(16);  var b = parseInt(document.getElementById('b').value).toString(16);  if(r.length < 2) { r = '0' + r; }   if(g.length < 2) { g = '0' + g; }   if(b.length < 2) { b = '0' + b; }   var rgb = '#'+r+g+b;    console.log('RGB: ' + rgb); connection.send(rgb); }</script></head><body>LED Control:<br/><br/>R: <input id=\"r\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>G: <input id=\"g\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>B: <input id=\"b\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/></body></html>");
    digitalWrite(LED_BUILTIN, 0);
}

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
    Serial.println(WiFi.softAPIP());

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
