#ifndef ROVDATATYPES_H
#define ROVDATATYPES_H

#include <QtGlobal>

struct RovControl
{
    qint8 axes[6] = {0,0,0,0,0,0};
    qint8 directions[10] = {1,1,1,1,1,1,1,1,1,1};
    float asf[6] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};//Axes Scale Factor
    quint16 buttons = 0;
    qint8 hats[4] = {0,0,0,0};

    RovControl() {}
};

struct RovDatagram
{
    quint8 version = 2;
    qint8 header = 0xAC;
    qint8 thrusterPower[10] = {0,0,0,0,0,0,0,0,0,0};
    qint8 cameraRotation[2] = {0,0}; // front, rear
    qint8 manipulator[2] = {0,0}; // open/close, rotate
    quint8 auxFlags = 0b00000000;
    qint8 camsel;
    RovDatagram(){}
};

struct RovTelemetry
{
    static const uint8_t header_telemetry = 0xAE;
    enum ErrorCode{

    };
    uint8_t header = 0;
    int8_t version = 2;
    float depth = 0.0f;
    float pitch = 0; //! -180/180;
    float yaw = 0; //! 0 - 360;
    float roll = 0; //! -180/180;
    float ammeter = 0.0f;
    float voltmeter = 0.0f;
//    int8_t manipulatorAngle = 0; //! -1 ccw/+1 cw
//    int8_t manipulatorState = 0; //! -1 close/+1 open
    int8_t cameraIndex = 0; //! 0 / 1 video multiplexer
    float temperature = 0.0f;
    RovTelemetry() {}
};



#endif // ROVDATATYPES_H
