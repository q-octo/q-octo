#pragma once

#include "Arduino.h"

namespace TaskCAN
{

  // public:
  void init();
  void loop();
  // private:
  bool isPowerMonitorPacket(uint32_t packetId);
  void onReceiveCanPacket(uint8_t packetSize, uint32_t packetId, uint8_t *packetData, bool extended);
}
