#ifndef ROVDATATYPES_H
#define ROVDATATYPES_H

#include "qfloat16.h"
#include <QtGlobal>

struct RovControl
{
    qint8 axes[6] = {0,0,0,0,0,0};
    qint8 directions[10] = {1,1,1,1,-1,1,-1,1,1,1};
    float asf[6] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};//Axes Scale Factor
    quint16 buttons = 0;
    qint8 hats[4] = {0,0,0,0};

    RovControl() {}
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

struct RovRegulatorsDatagram
{
    qint8 header = 0xAD;
    bool yawOn:1 = 0;
    bool depthOn:1 = 0;
    bool pitchOn:1 = 0;
    qfloat16 desYaw = 0.0f;
    qfloat16 desDepth = 0.0f;
    qfloat16 desPitch = 0.0f;
};

struct RovTelemetry
{
    static const uint8_t header_telemetry = 0xAE;
    enum ErrorCode{
        NoError,
        WrongCrc //TODO: maybe add smth else
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
