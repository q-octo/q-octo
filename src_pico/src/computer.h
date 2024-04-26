#pragma once

#include <Arduino.h>
#include "config.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task_data_manager.h"
#include "robot_state_generated.h"
#include "onboard_computer_rx_generated.h"
#include "onboard_computer_tx_generated.h"
#include "flatbuffer_serial_parser.h"
#include <flatbuffers/flatbuffer_builder.h>
#include "state_to_flatbuffer.h"

using namespace fbs;

namespace Computer
{
  // public:
  enum MessageType
  {
    STATE_UPDATE,
    CRSF_DATA,
    DISPLAY_BUTTON
  };
  typedef struct
  {
    MessageType type;
    union
    {
      DataManager::State state;
      DataManager::DisplayButton displayButton;
    } as;
  } Message;

  void receiveMessage(const Message &message);
  void init();
  void loop();


  // private:
  void serialiseState(const DataManager::State &state);
  void sendStateToComputer(const DataManager::State &state);
  void handleComputerTx(const OnboardComputerTx &computerTx);
  void sendTaskMessage(const DataManager::Message &message);
  void sendDisplayButtonToComputer(const DataManager::DisplayButton &displayButton);
  void parseIncomingSerialData();
  bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);
  Button serialiseDisplayButton(const DataManager::DisplayButton &displayButton);

}