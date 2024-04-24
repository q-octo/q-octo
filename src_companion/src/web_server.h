#pragma once

#include "Arduino.h"
#include "config.h"
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <LEAmDNS.h>
#include "ws_robot_generated.h"
#include "ws_update_generated.h"
#include <flatbuffers/flatbuffer_builder.h>
using namespace fbs;

class QOctoWebServer
{
public:
    QOctoWebServer() : apIP(192, 168, 4, 1) {}
    void loop();
    void start();
    void stop();
    void updateState(uint8_t *payload, size_t length)
    {
        assert(length <= CFG_STATE_MESSAGE_SIZE);
        stateBufferLength = length;
        memcpy(stateBuffer, payload, length);
        state = GetRobot(stateBuffer);
    }

private:
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
    void handleRoot();
    void restartWebSocket();

    void broadcastData();
    void sendDataToClient(uint8_t num);
    void onFileRoute(const struct packed_file *file);
    void processRemoteBinaryUpdate(uint8_t *payload, size_t length);
    const char *textToMIMEType(const char *text);

    uint8_t stateBuffer[CFG_STATE_MESSAGE_SIZE];
    uint8_t stateBufferLength = 0;
    const Robot *state = nullptr;
    IPAddress apIP;
    long lastWebSocketBroadcast = 0;

    WebServer *webServer;
    WebSocketsServer *webSocket;
    flatbuffers::FlatBufferBuilder fbb;
    bool webServerIsRunning = false;
    uint32_t lastWebSocketRestart = 0;

    bool pendingBroadcast = false;
};