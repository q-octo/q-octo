#pragma once

#include "Arduino.h"

void taskCAN(void *pvParameters);
void onReceiveCanPacket(uint8_t packetSize, uint32_t packetId, uint8_t *packetData,
                        bool extended);
