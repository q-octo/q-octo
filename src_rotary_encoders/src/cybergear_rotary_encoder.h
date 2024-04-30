#pragma once

#include <stdint.h>

typedef void (*TransmitCanCallback)(uint32_t packetId, uint8_t *packetData);


typedef struct {
    // TODO double check the data types
    float position;
    // -2048 to 2047
    float speed;
    float torque;

} RotaryEncoderResponse;

class CybergearRotaryEncoder
{
public:
    CybergearRotaryEncoder(
        // CAN2.0A (11-bit ID), packet length = 8 bytes
        TransmitCanCallback transmitCanCallback,
        uint8_t motorId)
        : _sendCanPacket(transmitCanCallback), _motorId(motorId) {}
    virtual ~CybergearRotaryEncoder();
    void enableExtendedMode();
    void startMotor();
    void stopMotor();
    void torqueControlMode();
    void speedTorqueControlMode();
    void positionSpeedTorqueTripleClosedLoopMode();
    void setMotorPositionToZero();
    void move(int16_t positionDegrees, int16_t speed, uint16_t positionGain, uint16_t positionDamping, uint16_t torque);
    void processMessage(RotaryEncoderResponse* response, uint8_t packetLength, uint32_t packetId, const uint8_t *packetData);


private:
    TransmitCanCallback _sendCanPacket;
    uint8_t _motorId;
};