#pragma once

#include "Arduino.h"
#include "config.h"
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <LEAmDNS.h>
#include "robot_state_generated.h"
#include "ws_update_generated.h"
#include <flatbuffers/flatbuffer_builder.h>
using namespace fbs;

class QOctoWebServer
{
public:
    QOctoWebServer() : apIP(192, 168, 4, 1) {
      // Initialises state & the flatbuffers builder
      updateState(RobotT());
    }
    void loop();
    void start();
    void stop();
    void updateState(const RobotT& robot) {
      state = robot;
      fbb.Reset();
      FinishRobotBuffer(fbb, Robot::Pack(fbb, &state));
    }

    bool getWebServerIsRunning() { return webServerIsRunning; }

private:
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
    void handleRoot();
    void restartWebSocket();

    void broadcastData();
    void sendDataToClient(uint8_t num);
    void onFileRoute(const struct packed_file *file);
    void processRemoteBinaryUpdate(uint8_t *payload, size_t length);
    static const char *textToMIMEType(const char *text);

    RobotT state;
    IPAddress apIP;
    unsigned long lastWebSocketBroadcast = 0;

    WebServer *webServer = nullptr;
    WebSocketsServer *webSocket = nullptr;
    DNSServer *dnsServer = nullptr;
    flatbuffers::FlatBufferBuilder fbb;
    bool webServerIsRunning = false;
    uint32_t lastWebSocketRestart = 0;

    bool pendingBroadcast = false;
};