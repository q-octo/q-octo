/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "web_server.h"
#include "companion_tx_generated.h"
#include "serial.h"

// Putting this in the header file breaks the build
#include "packed_fs.h"

#define AUTO_RESTART_WEBSOCKETS 0
#define RESTART_WEBSOCKETS_FREQUENCY (1000 * 15) // 15 seconds
#define WEBSOCKET_BROADCAST_FREQ 32

const char *QOctoWebServer::textToMIMEType(const char *text)
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

void QOctoWebServer::onFileRoute(const packed_file *p)
{
    digitalWrite(LED_BUILTIN, 1);
    // Split name on . to get MIME type
    String uri = webServer->uri();
    int lastDotPos = uri.lastIndexOf('.');
    String ext = (lastDotPos != -1) ? uri.substring(lastDotPos + 1) : String("");
    const char *mimeType = textToMIMEType(ext.c_str());
    webServer->send(200, mimeType, p->data, p->size);
    digitalWrite(LED_BUILTIN, 0);
}

void QOctoWebServer::start()
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
    Serial.printf("%s (rover.local)\n", WiFi.softAPIP().toString().c_str());

    webServer = new WebServer(80);
    webServer->on("/", [this]()
                  { handleRoot(); });
    webServer->onNotFound([this]()
                          { handleRoot(); });
    const struct packed_file *p;
    for (p = packed_files; p->name != nullptr; p++)
    {
        webServer->on(p->name, [p, this]()
                      { onFileRoute(p); });
    }
    webServer->begin();
    Serial.println("HTTP server started");

    webSocket = new WebSocketsServer(81);
    webSocket->onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
                       { webSocketEvent(num, type, payload, length); });
    webSocket->begin();
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

    dnsServer = new DNSServer();
    dnsServer->start(53, "*", apIP);

    webServerIsRunning = true;
}

void QOctoWebServer::broadcastData()
{
    Serial.println("Broadcasting data");
    webSocket->broadcastBIN(fbb.GetBufferPointer(), fbb.GetSize());
    lastWebSocketBroadcast = millis();
    pendingBroadcast = false;
}

void QOctoWebServer::sendDataToClient(uint8_t num)
{
    webSocket->sendBIN(num, fbb.GetBufferPointer(), fbb.GetSize());
}

void QOctoWebServer::stop()
{
    Serial.println("Stopping web server");
    MDNS.close();
    Serial.println("mDNS responder stopped");
    
    if (dnsServer != nullptr) {
        dnsServer->stop();
        delete dnsServer;
        dnsServer = nullptr;
        Serial.println("DNS server stopped");
    }

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

void QOctoWebServer::loop()
{

    if (!webServerIsRunning)
    {
        return;
    }
    webServer->handleClient();
    webSocket->loop();
    MDNS.update();
    dnsServer->processNextRequest();
    if (pendingBroadcast && millis() - lastWebSocketBroadcast > WEBSOCKET_BROADCAST_FREQ)
    {
        broadcastData();
    }

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

void QOctoWebServer::handleRoot()
{
    digitalWrite(LED_BUILTIN, 1);
    size_t htmlSize;
    const char *html = mg_unpack("/index.html", &htmlSize, nullptr);
    webServer->send(200, "text/html", html, htmlSize);
    digitalWrite(LED_BUILTIN, 0);
}

void QOctoWebServer::processRemoteBinaryUpdate(uint8_t *payload, size_t length)
{
    QOctoSerial::sendUpdateMessage(payload, length);
}

void QOctoWebServer::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    }
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket->remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        sendDataToClient(num);
    }
    break;
    case WStype_TEXT:
    {
        // This is unexpected.
        Serial.printf("[%u] got Text: %s\n", num, payload);
        break;
    }
    case WStype_BIN:
    {
        Serial.printf("[%u] got binary length: %u\n", num, length);
        hexdump(payload, length);
        processRemoteBinaryUpdate(payload, length);
        break;
    }
    default:
        Serial.println("[WARN] Unexpected websocket event");
        break;
    }
}

void QOctoWebServer::restartWebSocket()
{
    Serial.println("Restarting web socket server");
    webSocket->close();
    webSocket->begin();
}