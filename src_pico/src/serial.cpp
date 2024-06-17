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
#include "serial.h"
#include <flatbuffer_serial_parser.h>

void SerialTask::init()
{
    queue = xQueueCreate(10, sizeof(SendSerialData));
    computerSerial.begin(115200);
}

void SerialTask::loop()
{
    static SendSerialData data;
    if (xQueueReceive(queue, &data, 0) == pdTRUE)
    {
        memcpy(sendBuffer, data.data, data.size);
        switch (data.target)
        {
        case COMPANION:
        { 
            companionSerial.write(FlatbufferSerialParser::START_BYTE);
            if (companionSerial.write(sendBuffer, data.size)) {
                Serial.println("SENT TO COMPANION");
            } else  {
                // TODO we're hitting this branch :()
                Serial.println("FAILED TO SEND TO COMPANION");
            }
            break;
        }
        case ONBOARD_COMPUTER:
            computerSerial.write(sendBuffer, data.size);
            break;
        }
    }
}