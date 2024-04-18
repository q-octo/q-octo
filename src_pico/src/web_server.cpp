
#include <Arduino.h>
#include "web_server.h"
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <LEAmDNS.h>
#include "packed_fs.h"

#define AUTO_RESTART_WEBSOCKETS 0
#define RESTART_WEBSOCKETS_FREQUENCY 1000 * 15 // 15 seconds

IPAddress apIP(192, 168, 4, 1);
WebServer *webServer;
WebSocketsServer *webSocket;
// QueueHandle_t webServerQueue;

bool webServerIsRunning = false;
uint32_t lastWebSocketRestart = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void handleRoot();
void restartWebSocket();

void WSWebServer::init()
{
    // // Initialise the queue
    // if (webServerQueue == nullptr)
    // {
    //     webServerQueue = xQueueCreate(10, sizeof(WSWebServer::Message));
    //     if (webServerQueue == nullptr)
    //     {
    //         Serial.println("Failed to create webServerQueue");
    //         return;
    //     }
    // }
}

const char *textToMIMEType(const char *text)
{
    // Handle css, js, html, svg
    if (strcmp(text, "css") == 0)
    {
        return "text/css";
    }
    else if (strcmp(text, "js") == 0)
    {
        return "application/javascript";
    }
    else if (strcmp(text, "html") == 0)
    {
        return "text/html";
    }
    else if (strcmp(text, "svg") == 0)
    {
        return "image/svg+xml";
    }
    else
    {
        return "text/plain";
    }
}

void onFileRoute(const packed_file *p)
{
    digitalWrite(LED_BUILTIN, 1);
    // Split name on . to get MIME type
    char *name = (char *)webServer->uri().c_str();
    String uri = webServer->uri();
    int lastDotPos = uri.lastIndexOf('.');
    String ext = (lastDotPos != -1) ? uri.substring(lastDotPos + 1) : String("");
    
    // Serial.println("File extension: " + ext);
    const char *mimeType = textToMIMEType(ext.c_str());
    // Serial.println("Serving file: " + webServer->uri() + " with MIME type: " + mimeType);
    webServer->send(200, mimeType, p->data, p->size);
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
    Serial.println(WiFi.softAPIP() + " (rover.local)");

    webServer = new WebServer(80);
    webServer->enableCORS(true);
    webServer->on("/", handleRoot);
    const struct packed_file *p;
    for (p = packed_files; p->name != NULL; p++)
    {
        webServer->on(p->name, [p]()
                      { onFileRoute(p); });
    }
    webServer->begin();
    Serial.println("HTTP server started");

    webSocket = new WebSocketsServer(81);
    webSocket->begin();
    webSocket->onEvent(webSocketEvent);
    Serial.println("Web socket server started");

    // Setup MDNS responder (creates rover.local domain within the AP)
    if (!MDNS.begin("rover"))
    {
        Serial.println("Error setting up MDNS responder!");
    }
    else
    {
        Serial.println("mDNS responder started");
        // Add service to mDNS
        MDNS.addService("http", "tcp", 80);
        MDNS.addService("ws", "tcp", 81);
    }

    webServerIsRunning = true;
}

void WSWebServer::stop()
{
    Serial.println("Stopping web server");
    MDNS.close();
    Serial.println("mDNS responder stopped");

    if (webSocket != nullptr)
    {
        webSocket->close();
        delete webSocket;
        webSocket = nullptr;
        Serial.println("Web socket server stopped");
    }

    if (webServer != nullptr)
    {
        webServer->close();
        delete webServer;
        webServer = nullptr;
        Serial.println("HTTP server stopped");
    }

    if (WiFi.connected())
    {
        WiFi.softAPdisconnect();
        Serial.println("AP disconnected (web server stopped)");
    }

    webServerIsRunning = false;
}

void WSWebServer::loop()
{
    // TODO xQueueReceive is NOT allowed here (breaks xSemaphoreTake)
    // static WSWebServer::Message message;
    // BaseType_t xStatus = xQueueReceive(webServerQueue, &message, pdMS_TO_TICKS(1));
    // const bool messageReceived = xStatus == pdPASS;
    // if (messageReceived)
    // {
    //     switch (message.type)
    //     {
    //     case WSWebServer::MessageType::ENABLE:
    //         Serial.println("Received web server ENABLE message");
    //         WSWebServer::start();
    //         break;
    //     case WSWebServer::MessageType::DISABLE:
    //         Serial.println("Received web server DISABLE message");
    //         WSWebServer::stop()
    //         break;
    //     }
    // }

    if (webServerIsRunning)
    {
        webServer->handleClient();
        webSocket->loop();
        MDNS.update();
#if AUTO_RESTART_WEBSOCKETS
        const uint32_t currentMs = millis();
        if (lastWebSocketRestart == 0)
        {
            // First run
            lastWebSocketRestart = currentMs;
        }
        if (currentMs - lastWebSocketRestart > RESTART_WEBSOCKETS_FREQUENCY)
        {
            restartWebSocket();
            lastWebSocketRestart = currentMs;
        }
#endif
    }
}

void handleRoot()
{
    digitalWrite(LED_BUILTIN, 1);
    size_t htmlSize;
    const char *html = mg_unpack("/index.html", &htmlSize, nullptr);
    webServer->send(200, "text/html", html, htmlSize);
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
        IPAddress ip = webSocket->remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket->sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);

        if (strcmp((char *)payload, "#000000") == 0)
        {
            Serial.println("restarting web server");
            WSWebServer::stop();
            WSWebServer::start();
        }

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

void restartWebSocket()
{
    Serial.println("Restarting web socket server");

    webSocket->close();
    webSocket->begin();
}
