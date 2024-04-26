#pragma once

#include <Arduino.h>
#include "config.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task_data_manager.h"
#include "robot_state_generated.h"
#include "ws_update_generated.h"
#include "companion_rx_generated.h"
#include "companion_tx_generated.h"
#include "flatbuffer_serial_parser.h"
#include <flatbuffers/flatbuffer_builder.h>

using namespace fbs;

class Companion {
public:
  enum MessageType { ENABLE_WEB_SERVER, DISABLE_WEB_SERVER, STATE_UPDATE };
  typedef struct {
    MessageType type;
    union {
      TaskMessage::State state;
    } as;
  } Message;

  static void receiveMessage(const Message &message);
  static void loop();
  static void companionProducerTask(void *pvParameters);
  static void companionConsumerTask(void *pvParameters);

  static inline QueueHandle_t companionQueue = nullptr;
  static inline SerialPIO companionSerial =
      SerialPIO(CFG_COMPANION_UART_TX, CFG_COMPANION_UART_RX, 32);

private:
  static void sendToCompanion(const uint8_t *data, size_t length);
  static void serialiseState(const TaskMessage::State &state);
  static void sendStateToCompanion(const TaskMessage::State &state);
  static void handleCompanionTx(const CompanionTx &companionTx);
  static void handleUpdateMessage(const Update &update);
  static void handleButtonPressedMessage(const ButtonPressed &buttonPressed);
  static void sendTaskMessage(const TaskMessage::Message &message);
  static void parseIncomingSerialData();
  static bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);

  // 1024 is the default size, but it will grow automatically.
  static inline flatbuffers::FlatBufferBuilder fbb = flatbuffers::FlatBufferBuilder(1024);
  static inline FlatbufferSerialParser fbSerialParser = FlatbufferSerialParser(companionSerial, verifyIncomingFlatbuffer);
};