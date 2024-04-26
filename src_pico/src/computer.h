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

class Computer {
public:
  enum MessageType { STATE_UPDATE, CRSF_DATA, DISPLAY_BUTTON};
  typedef struct {
    MessageType type;
    union {
      TaskMessage::State state;
      TaskMessage::DisplayButton displayButton;
    } as;
  } Message;


  static void receiveMessage(const Message &message);
  static void loop();
  static void taskProducer(void *pvParameters);
  static void taskConsumer(void *pvParameters);

  static inline QueueHandle_t computerQueue = nullptr;
  static inline SerialUART computerSerial = Serial2;

private:
  static void serialiseState(const TaskMessage::State &state);
  static void sendStateToComputer(const TaskMessage::State &state);
  static void handleComputerTx(const OnboardComputerTx &computerTx);
  static void sendTaskMessage(const TaskMessage::Message &message);
  static void sendDisplayButtonToComputer(const TaskMessage::DisplayButton &displayButton);
  static void parseIncomingSerialData();
  static bool verifyIncomingFlatbuffer(flatbuffers::Verifier &verifier);
  static Button serialiseDisplayButton(const TaskMessage::DisplayButton &displayButton);

  // 1024 is the default size, but it will grow automatically.
  static inline flatbuffers::FlatBufferBuilder fbb = flatbuffers::FlatBufferBuilder(1024);
  static inline FlatbufferSerialParser fbSerialParser = FlatbufferSerialParser(computerSerial, verifyIncomingFlatbuffer);
};