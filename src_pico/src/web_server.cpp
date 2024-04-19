#include "config.h"
#include <Arduino.h>
#include "web_server.h"
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <LEAmDNS.h>
#include "packed_fs.h"
#include "ws_robot_generated.h"
#include "ws_update_generated.h"
#include <flatbuffers/flatbuffer_builder.h>
using namespace fbs;

#define AUTO_RESTART_WEBSOCKETS 0
#define RESTART_WEBSOCKETS_FREQUENCY 1000 * 15 // 15 seconds
static const int WEBSOCKET_BROADCAST_FREQ = 500;
long lastWebSocketBroadcast = 0;

IPAddress apIP(192, 168, 4, 1);
WebServer *webServer;
WebSocketsServer *webSocket;
flatbuffers::FlatBufferBuilder fbb;
// Requested state
bool webServerEnabled = false;
// Actual state
bool webServerIsRunning = false;
uint32_t lastWebSocketRestart = 0;

// Visible to other tasks
bool pendingBroadcast = false;
SemaphoreHandle_t xWebServerMutex;
QueueHandle_t webServerQueue;
TaskMessage::State state;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void handleRoot();
void restartWebSocket();

void serialiseFlatbuffer(flatbuffers::FlatBufferBuilder &builder)
{

    // TODO move these varaibles into TaskMessage::State
    ControlSource control_source = ControlSource_RC;
    Status status = Status_OK;
    float max_speed = 0.0f;
    int reference_wheel_angle = 0;
    std::string motor_error_code;
    bool wheels_folded = false;
    bool enable_rover = false;

    auto motor1 = CreateMotor(builder, state.motors.left.temperature, state.motors.left.RPM, state.motors.left.position);
    auto motor2 = CreateMotor(builder, state.motors.right.temperature, state.motors.right.RPM, state.motors.right.position);
    auto motors = CreateMotors(builder, motor1, motor2);

    auto robot = CreateRobot(
        builder,
        state.batteryCount,
        control_source,
        status,
        motors,
        state.battery.voltage,
        state.battery.current,
        state.battery.fuel,
        0, // TODO fix schema (rssi = signal strength)
        0,
        max_speed,
        state.lowVoltageThreshold,
        state.criticalVoltageThreshold,
        reference_wheel_angle,
        builder.CreateString(motor_error_code),
        wheels_folded,
        enable_rover);

    builder.Finish(robot);
}

void webServerConsumerTask(void *pvParameters)
{
    // CANNOT INTERACT WITH THE WEB SERVER APIs HERE
    (void)pvParameters; // To avoid warnings

    static WSWebServer::Message message;
    Serial.println("Web server consumer task started");

#if !CFG_ENABLE_WEB_SERVER
    Serial.println("Web server is disabled, blocking indefinitely");
    for (;;)
    {
        xQueueReceive(webServerQueue, &message, portMAX_DELAY);
    }
#endif

    for (;;)
    {
        if (xQueueReceive(webServerQueue, &message, portMAX_DELAY))
        {
            switch (message.type)
            {
            case WSWebServer::MessageType::ENABLE:
                Serial.println("Received web server ENABLE message");
                webServerEnabled = true;
                break;
            case WSWebServer::MessageType::DISABLE:
                Serial.println("Received web server DISABLE message");
                webServerEnabled = false;
                break;
            case WSWebServer::MessageType::STATE_UPDATE:
            {
                if (xSemaphoreTake(xWebServerMutex, portMAX_DELAY) == pdTRUE)
                {
                    state = message.as.state;
                    pendingBroadcast = true;
                    xSemaphoreGive(xWebServerMutex);
                }
            }
            }
        }
    }
}

void WSWebServer::init()
{
    xWebServerMutex = xSemaphoreCreateMutex();
    if (xWebServerMutex == nullptr)
    {
        Serial.println("[WARN} Failed to create mutex");
    }
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
    String uri = webServer->uri();
    int lastDotPos = uri.lastIndexOf('.');
    String ext = (lastDotPos != -1) ? uri.substring(lastDotPos + 1) : String("");
    const char *mimeType = textToMIMEType(ext.c_str());
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
    webServerEnabled = true;
}

void broadcastData()
{
    if (xSemaphoreTake(xWebServerMutex, portMAX_DELAY) == pdTRUE)
    {
        fbb.Reset(); // To avoid forever growing buffer
        serialiseFlatbuffer(fbb);
        webSocket->broadcastBIN(fbb.GetBufferPointer(), fbb.GetSize());
        pendingBroadcast = false;
        lastWebSocketBroadcast = millis();
        xSemaphoreGive(xWebServerMutex);
    }
}

void sendDataToClient(uint8_t num)
{
    if (xSemaphoreTake(xWebServerMutex, portMAX_DELAY) == pdTRUE)
    {
        fbb.Reset(); // To avoid forever growing buffer
        serialiseFlatbuffer(fbb);
        webSocket->sendBIN(num, fbb.GetBufferPointer(), fbb.GetSize());
        xSemaphoreGive(xWebServerMutex);
    }
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
    webServerEnabled = false;
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
        if (pendingBroadcast && millis() - lastWebSocketBroadcast > WEBSOCKET_BROADCAST_FREQ)
        {
            broadcastData();
        }
        broadcastData();

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

    if (webServerEnabled && !webServerIsRunning)
    {
        start();
    }
    else if (!webServerEnabled && webServerIsRunning)
    {
        stop();
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

void sendTaskMessage(TaskMessage::Message *message)
{
    // TODO hopefully this doesn't cause a crash. If it does, we need a separate
    // task for sending messages to the data manager.
    xQueueSend(dataManagerQueue, message, 0);
}

void processRemoteBinaryUpdate(uint8_t *payload, size_t length)
{
    auto update = GetUpdate(payload);
    static TaskMessage::Message taskMessage;
    switch (update->update_type())
    {
    case UpdateUnion::UpdateUnion_UpdateBatteries:
    {
        auto updateBatteries = update->update_as_UpdateBatteries();
        taskMessage = {
            .type = TaskMessage::Type::SET_BATTERY_COUNT,
            .as = {.batteryCount = updateBatteries->batteries()},
        };
        sendTaskMessage(&taskMessage);
        break;
    }
    case UpdateUnion::UpdateUnion_UpdateLowVoltageThreshold:
    {
        auto updateLowVoltageThreshold = update->update_as_UpdateLowVoltageThreshold();
        taskMessage = {
            .type = TaskMessage::Type::SET_LOW_VOLTAGE_THRESHOLD,
            .as = {
                .voltageThreshold = updateLowVoltageThreshold->low_voltage_threshold(),
            },
        };
        sendTaskMessage(&taskMessage);
        break;
    }
    case UpdateUnion::UpdateUnion_UpdateCriticalVoltageThreshold:
    {
        auto updateCriticalVoltageThreshold = update->update_as_UpdateCriticalVoltageThreshold();
        taskMessage = {
            .type = TaskMessage::Type::SET_CRITICAL_VOLTAGE_THRESHOLD,
            .as = {
                .voltageThreshold = updateCriticalVoltageThreshold->critical_voltage_threshold(),
            },
        };
        sendTaskMessage(&taskMessage);
        break;
    }
    case UpdateUnion::UpdateUnion_UpdateReferenceWheelAngle:
    {
        auto updateReferenceWheelAngle = update->update_as_UpdateReferenceWheelAngle();
        // TODO implement
        // robot.reference_wheel_angle = updateReferenceWheelAngle->reference_wheel_angle();
        break;
    }
    case UpdateUnion::UpdateUnion_UpdateFoldWheels:
    {
        // robot.wheels_folded = true;
        taskMessage = {.type = TaskMessage::Type::FOLD_WHEELS};
        sendTaskMessage(&taskMessage);
        break;
    }
    case UpdateUnion::UpdateUnion_UpdateEnableRover:
    {
        auto updateEnableRover = update->update_as_UpdateEnableRover();
        // TODO implement
        // robot.enable_rover = updateEnableRover->enable_rover();
        break;
    }
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
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
        Serial.printf("[%u] get Text: %s\n", num, payload);
        break;
    }
    case WStype_BIN:
    {
        Serial.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);
        processRemoteBinaryUpdate(payload, length);
        break;
    }
    }
}

void restartWebSocket()
{
    Serial.println("Restarting web socket server");
    webSocket->close();
    webSocket->begin();
}
