#pragma once

#include "Arduino.h"

class TaskCAN
{
public:
  static void init();
  static void loop();
private:
  static bool isPowerMonitorPacket(uint32_t packetId);
  static void onReceiveCanPacket(uint8_t packetSize, uint32_t packetId, uint8_t *packetData, bool extended);
};
