#include "cybergear_rotary_encoder.h"

CybergearRotaryEncoder::~CybergearRotaryEncoder() {}

void CybergearRotaryEncoder::enableExtendedMode()
{
    uint8_t data[8] = {0x00};
    _sendCanPacket(_motorId, data);
}

void CybergearRotaryEncoder::startMotor()
{
    uint8_t data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC};
    _sendCanPacket(_motorId, data);
}

void CybergearRotaryEncoder::stopMotor()
{
    uint8_t data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD};
    _sendCanPacket(_motorId, data);
}

void CybergearRotaryEncoder::torqueControlMode()
{
    uint8_t data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9};
    _sendCanPacket(_motorId, data);
}

void CybergearRotaryEncoder::speedTorqueControlMode()
{
    uint8_t data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA};
    _sendCanPacket(_motorId, data);
}

void CybergearRotaryEncoder::positionSpeedTorqueTripleClosedLoopMode()
{
    uint8_t data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB};
    _sendCanPacket(_motorId, data);
}

void CybergearRotaryEncoder::setMotorPositionToZero()
{
    uint8_t data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE};
    _sendCanPacket(_motorId, data);
}

void CybergearRotaryEncoder::move(uint16_t position, uint16_t speed, uint16_t positionGain, uint16_t positionDamping, uint16_t torque)
{
    // TODO implement this (rn it is just a sample payload)
    uint8_t data[8] = {0xC0, 0x00, 0xC0, 0x01, 0x00, 0x02, 0x0C, 0x00};
    _sendCanPacket(_motorId, data);
}
void CybergearRotaryEncoder::processMessage(RotaryEncoderResponse *response, uint8_t packetLength, uint32_t packetId, const uint8_t *packetData)
{
    if (response == nullptr)
    {
        return;
    }
    const uint8_t motorId = packetData[0];
    if (motorId != _motorId)
    {
        return;
    }

    const bool extendedMode = packetLength == 8;
    response->position = (packetData[1] << 8) + packetData[2];
    if (extendedMode)
    {
        response->position += (packetData[7] << 16) + (packetData[6] << 24);
    }
    response->speed = (int16_t)(((packetData[3] & 0xFF) << 4) + ((packetData[4] & 0xF0) >> 4)) - 2048;
    response->torque = (int16_t)(((packetData[4] & 0x0F) << 8) + packetData[5]) - 2048;
}