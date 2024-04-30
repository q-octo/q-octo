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

// Speed is between -200 and 200
void CybergearRotaryEncoder::move(int16_t positionDegrees, int16_t speed, uint16_t positionGain, uint16_t positionDamping, uint16_t torque)
{
    uint16_t position = (positionDegrees / 360) * 0x8000 + 0x8000;
    uint16_t _speed = (speed / 200) * 2048 + 2048;
    uint8_t data[8] = {0xC0, 0x00, 0xC0, 0x01, 0x00, 0x02, 0x0C, 0x00};
    _sendCanPacket(_motorId, data);
    return;
    // return;
    // TODO implement this (rn it is just a sample payload)
    // auto _speed = (0x864 << 4) & 0x01;
    // uint8_t data[8] = {0xC0, 0x00, (_speed >> 8) && 0xFF, _speed && 0xFF, 0x00, 0x02, 0x0C, 0x00};
    uint8_t data2[8] = {
        // Position (top 8)
        position >> 8,
        // Position (bottom 8)
        position & 0xFF,
        // Speed (top 4)
        _speed >> 4,
        // Speed (bottom 4), position gain (top 4)
        ((_speed & 0x0F) << 4) & ((positionGain >> 8) & 0x0F),
        // position gain (bottom 8)
        positionGain & 0xFF,
        // position damping (top 8)
        positionDamping >> 8,
        // position damping (bottom 4), torque (top 4)
        ((positionDamping & 0x0F) << 4) & ((torque >> 8) & 0x0F),
        // torque (bottom 8)
        torque & 0xFF,
    };
    _sendCanPacket(_motorId, data2);
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

    // 0x 01 A8 B0 7F E7 FF
    //     0  1 2  3   4 5

    // 0xA8B0 = 43184

    const bool extendedMode = packetLength == 8;

    auto rawPosition = (packetData[1] << 8) + packetData[2];
    auto rawSpeed = (packetData[3] << 4) + ((packetData[4] & 0xF0) >> 4);
    auto rawTorque = ((packetData[4] & 0x0F) << 8) + packetData[5];
    response->position = ((((float)rawPosition - 0x8000) / 0x8000) * 360);
    response->speed = ((((float)rawSpeed - 0x800) / 0x800) * 200);
    response->torque = ((((float)rawTorque - 0x800) / 0x800) * 200);

    // // (0xA8B0 - 0x8000)/0x8000
    // response->position = ((((packetData[1] << 8) + packetData[2]) - 0x8000) / 0x8000) * 360;
    // // if (extendedMode)
    // // {
    // //     response->position += (packetData[7] << 16) + (packetData[6] << 24);
    // // }
    // response->speed = (((((packetData[3] & 0xFF) << 4) + ((packetData[4] & 0xF0) >> 4)) - 0x800) / 0x800);
    // response->torque = (((((packetData[4] & 0x0F) << 8) + packetData[5]) - 0x800) / 0x800);
}