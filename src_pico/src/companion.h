#pragma once

#include <Arduino.h>
#include "config.h"

#include "task_data_manager.h"

#include "robot_state_generated.h"
#include "ws_update_generated.h"
#include "companion_rx_generated.h"
#include "companion_tx_generated.h"
#include "flatbuffer_serial_parser.h"
#include <flatbuffers/flatbuffer_builder.h>
#include "state_to_flatbuffer.h"

using namespace fbs;

class Companion
{
public:
  enum MessageType
  {
    ENABLE_WEB_SERVER,
    DISABLE_WEB_SERVER,
    STATE_UPDATE
  };

  typedef struct
  {
    MessageType type;
    union
    {
      DataManager::State state;
    } as;
  } Message;

  static void init();
  static void receiveMessage(const Message &message);
  static void loop();

private:
  static void serialiseState(const DataManager::State &state);
  static void sendStateToCompanion(const DataManager::State &state);
  static void handleCompanionTx(const CompanionTx &companionTx);
  static void handleUpdateMessage(const Update &update);
  static void handleButtonPressedMessage(const ButtonPressed &buttonPressed);
  static void sendTaskMessage(const DataManager::Message &message);
  static void parseIncomingSerialData();
  static bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);

  static inline SerialPIO companionSerial = SerialPIO(CFG_COMPANION_UART_TX, CFG_COMPANION_UART_RX, 2048);
  // static inline SerialPIO companionSerial = SerialPIO(SerialPIO::NOPIN, CFG_COMPANION_UART_RX, 2048);
  // 1024 is the default size, but it will grow automatically.
  static inline flatbuffers::FlatBufferBuilder fbb = flatbuffers::FlatBufferBuilder(2048);
  static inline FlatbufferSerialParser fbSerialParser = FlatbufferSerialParser(companionSerial, verifyIncomingFlatbuffer);
  static inline uint32_t msSinceLastBroadcast = 0;
};