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
#include "state_to_flatbuffer.h"

using namespace fbs;

namespace Companion
{
  // public:
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

  void init();
  void receiveMessage(const Message &message);
  void loop();


  // private:
  void serialiseState(const DataManager::State &state);
  void sendStateToCompanion(const DataManager::State &state);
  void handleCompanionTx(const CompanionTx &companionTx);
  void handleUpdateMessage(const Update &update);
  void handleButtonPressedMessage(const ButtonPressed &buttonPressed);
  void sendTaskMessage(const DataManager::Message &message);
  void parseIncomingSerialData();
  bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);

}