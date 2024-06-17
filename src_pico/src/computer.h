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