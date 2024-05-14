#pragma once

#include <Arduino.h>
#include "config.h"
#include "task_data_manager.h"
#include "robot_state_generated.h"
#include "onboard_computer_rx_generated.h"
#include "onboard_computer_tx_generated.h"
#include "flatbuffer_serial_parser.h"
#include <flatbuffers/flatbuffer_builder.h>
#include "state_to_flatbuffer.h"

class Computer
{
public:
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

  static void receiveMessage(const Message &message);
  static void init();
  static void loop();


private:
  static void serialiseState(const DataManager::State &state);
  static void sendStateToComputer(const DataManager::State &state);
  static void handleComputerTx(const OnboardComputerTx &computerTx);
  static void sendDisplayButtonToComputer(const DataManager::DisplayButton &displayButton);
  static void parseIncomingSerialData();
  static bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);
  static inline Button serialiseDisplayButton(const DataManager::DisplayButton &displayButton);
  static inline SerialUART computerSerial = Serial2;
  // 1024 is the default size, but it will grow automatically.
  static inline flatbuffers::FlatBufferBuilder fbb = flatbuffers::FlatBufferBuilder(1024);
  static inline FlatbufferSerialParser fbSerialParser = FlatbufferSerialParser(computerSerial, verifyIncomingFlatbuffer);
};