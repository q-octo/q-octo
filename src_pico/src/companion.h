#pragma once

#include "config.h"
#include "task_data_manager.h"
#include "ws_robot_generated.h"
#include "ws_update_generated.h"
#include <flatbuffers/flatbuffer_builder.h>

class Companion {
public:
  enum MessageType { ENABLE_WEB_SERVER, DISABLE_WEB_SERVER, STATE_UPDATE };
  typedef struct {
    MessageType type;
    union {
      TaskMessage::State state;
    } as;
  } Message;

  static void accept(Message &message);
  static void loop();

  static QueueHandle_t companionQueue;
  static void companionProducerTask(void *pvParameters);
  static void companionConsumerTask(void *pvParameters);
  static inline SerialPIO companionSerial =
      SerialPIO(CFG_COMPANION_UART_TX, CFG_COMPANION_UART_RX, 32);

  static uint8_t serialBuffer[255];

private:
  static void sendToCompanion(uint8_t *data, size_t length);
  static void serialiseState(TaskMessage::State &state);
  static void sendStateToCompanion(TaskMessage::State &state);
  static void handleUpdateMessage(uint8_t *payload, size_t length);
  static void sendTaskMessage(TaskMessage::Message &message);

  static flatbuffers::FlatBufferBuilder fbb;
};