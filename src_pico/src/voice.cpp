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
#include "voice.h"

void Voice::init()
{
    voiceSerial.begin(9600);
    setVolume(0x1E);
}

void Voice::announce(Announcement announcement)
{
    playTrack(announcement);
}

void Voice::setVolume(uint8_t vol)
{
    uint8_t volume[5] = {0xAA, 0x13, 0x01, vol, vol + 0xBE};
    voiceSerial.write(volume, 5);
}

// If this works then we can replace the other playTrack function
void Voice::playTrackDocumented(uint16_t track)
{
    uint8_t play[6] = {0xAA, 0x07, 0x02, (track >> 8) & 0xFF, track & 0xFF, 0};
    uint8_t checksum = 0;
    for (int i = 0; i < 5; i++)
    {
        checksum += play[i];
    }
    play[5] = checksum;
    voiceSerial.write(play, 6);
}

void Voice::playTrack(uint8_t track)
{
    // playTrackDocumented(1);
    unsigned char play[6] = {0xAA, 0x07, 0x02, 0x00, track, track + 0xB3};
    voiceSerial.write(play, 6);
}