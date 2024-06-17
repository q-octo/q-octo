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
      
      if (webServerIsRunning != state.web_server_enabled) {
        if (state.web_server_enabled) {
          start();
        } else {
          stop();
        }
      }
      pendingBroadcast = true;
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