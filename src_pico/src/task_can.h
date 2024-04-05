#pragma once

#include "Arduino.h"

void taskCAN(void *pvParameters);
void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData,
                        bool extended);
