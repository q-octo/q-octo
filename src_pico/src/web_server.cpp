
#include <Arduino.h>
#include "web_server.h"
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

/*
The web server is off by default.
When instructed, it will be started.
It can be stopped.
For now, we'll assume that one device will be connected at any time?? hmm.
Anyway, it will establish a web socket connection and from there we can send
commands back and fourth as we wish.
Think back to the distributed systems & networks coursework, it's like this.
Create a list of instructions and the shape of their associated payload.

First action will likely be the server sending all current configuration info.
I imagine that a key-value system will work for all the properties to be
managed, this way we can add new configurations with minimal changes everywhere.


*/

#define USE_SERIAL Serial

int status = WL_IDLE_STATUS; // the WiFi server status
WiFiClass wifi;
const byte DNS_PORT = 53;
// IPAddress apIP(172, 217, 28, 1);
IPAddress apIP(192, 168, 0, 1);
DNSServer dnsServer;
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

    // wifi.addAP("paganello", "20242024");
    webServerInitialised = true;
    // USE_SERIAL.println("Web server initialised");
}

void handleRoot()
{
    digitalWrite(LED_BUILTIN, 1);
    webServer.send(200, "text/html", "<html><head><script>var connection = new WebSocket('ws://192.168.0.1:81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};function sendRGB() {  var r = parseInt(document.getElementById('r').value).toString(16);  var g = parseInt(document.getElementById('g').value).toString(16);  var b = parseInt(document.getElementById('b').value).toString(16);  if(r.length < 2) { r = '0' + r; }   if(g.length < 2) { g = '0' + g; }   if(b.length < 2) { b = '0' + b; }   var rgb = '#'+r+g+b;    console.log('RGB: ' + rgb); connection.send(rgb); }</script></head><body>LED Control:<br/><br/>R: <input id=\"r\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>G: <input id=\"g\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>B: <input id=\"b\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/></body></html>");
    digitalWrite(LED_BUILTIN, 0);
}

void WSWebServer::start()
{
    if (webServerIsRunning)
    {
        return;
    }
    USE_SERIAL.println("Starting web server");
    // if (!webServerInitialised)
    // {
    //     initWebServer();
    // }
    wifi.mode(WIFI_AP);
    wifi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Q-Octo Diagnostics");

    // if DNSServer is started with "*" for domain name, it will reply with
    // provided IP to all DNS request
    dnsServer.start(DNS_PORT, "rover.local", apIP);  
    

    // replay to all requests with same HTML
    webServer.onNotFound(handleRoot);
    webServer.begin();

    // while (wifi.beginAP("paganello", "20242024") != WL_CONNECTED)
    // {
    //     delayMicroseconds(100000); // 100ms
    // }
    USE_SERIAL.println("setup AP");
    webSocket.begin();
    USE_SERIAL.println("Web socket started");
    webSocket.onEvent(webSocketEvent);
    USE_SERIAL.println("Web socket event set");

    webServer.on("/", handleRoot);
    webServer.begin();

    // handle index
    // wifiServer.on("/", []()
    //               {
    //     // send index.html
    //     wifiServer.send(200, "text/html", "<html><head><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};function sendRGB() {  var r = parseInt(document.getElementById('r').value).toString(16);  var g = parseInt(document.getElementById('g').value).toString(16);  var b = parseInt(document.getElementById('b').value).toString(16);  if(r.length < 2) { r = '0' + r; }   if(g.length < 2) { g = '0' + g; }   if(b.length < 2) { b = '0' + b; }   var rgb = '#'+r+g+b;    console.log('RGB: ' + rgb); connection.send(rgb); }</script></head><body>LED Control:<br/><br/>R: <input id=\"r\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>G: <input id=\"g\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>B: <input id=\"b\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/></body></html>"); });

    // wifiServer.begin();

    webServerIsRunning = true;
}

void WSWebServer::stop()
{
    if (!webServerIsRunning)
    {
        return;
    }
    webSocket.close();
    webServerIsRunning = false;
}

void WSWebServer::loop()
{
    dnsServer.processNextRequest();
    webServer.handleClient();
    webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{

    switch (type)
    {
    case WStype_DISCONNECTED:
        USE_SERIAL.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
        USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);

        // send message to client
        // webSocket.sendTXT(num, "message here");

        // send data to all connected clients
        // webSocket.broadcastTXT("message here");
        break;
    case WStype_BIN:
        USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);

        // send message to client
        // webSocket.sendBIN(num, payload, length);
        break;
    }
}

/*
WiFiClient client = wifiServer.accept();
    if (client)
    {
        while (client.connected())
        {
            if (client.available())
            {
                USE_SERIAL.println("Connected to /");
                char c = client.read();
                // Process the request and send a response
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();
                client.println("<!DOCTYPE HTML>");
                client.println("<html>");
                // Add your HTML content here
                client.println("Something works :)");
                client.println("</html>");
                break;
            }
        }
        // Can probably remove the delay
        delay(1);
        client.stop();
    }
*/