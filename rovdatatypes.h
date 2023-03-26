#ifndef ROVDATATYPES_H
#define ROVDATATYPES_H

#include "qfloat16.h"
#include <QtGlobal>

struct RovControlState
{
    qint8 axes[6] = {0,0,0,0,0,0};
    qint8 directions[10] = {1,1,1,1,-1,1,-1,1,1,1};
    float asf[6] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};//Axes Scale Factor
    quint16 buttons = 0;
    qint8 hats[4] = {0,0,0,0};

    RovControlState() {}
};

struct RovControlDatagram
{
    qint8 header = 0xAC;
    quint8 version = 2;
    qint8 thrusterPower[10] = {0,0,0,0,0,0,0,0,0,0};
    qint8 cameraRotation[2] = {0,0}; // front, rear
    qint8 manipulator[2] = {0,0}; // open/close, rotate
    qint8 camsel = false;
    RovControlDatagram(){}
};

struct RovAuxDatagram
{
    qint8 header = 0xAD;
    qint8 auxFlags = 0b00000000;
    float dDepth = 0;
    float dYaw = 0;
    float dRoll = 0;
    float dPitch = 0;
    RovAuxDatagram(){};
};

struct RovTelemetry
{
    static const uint8_t header_telemetry = 0xAE;
    enum ErrorCode{
        NoError,
        WrongCrc //TODO: implement
    };
    uint8_t header = 0;
    int8_t version = 2;
    float depth = 0.0f;
    float pitch = 0; //! -180/180;
    float yaw = 0; //! 0 - 360;
    float roll = 0; //! -180/180;
    float current = 0.0f;
    float voltmeter = 0.0f;
    int8_t cameraIndex = 0; //! 0 / 1 video multiplexer
    float temperature = 0.0f;
    ErrorCode ec = NoError;
    RovTelemetry() {}
};



#endif // ROVDATATYPES_H
