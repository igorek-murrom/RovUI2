#include "rovdataparser.h"
#include "joystick.h"

RovDataParser::RovDataParser(QObject *parent)
    : QObject{parent}
{

}


void RovDataParser::doPrepareDatagram(Joystick rc){
    RovDatagram *out = new RovDatagram();
    qint8 x = rc.axes[0] * rc.asf[0] * rc.baseASF[0] * rc.directions[0];// left-right
    qint8 y = rc.axes[1] * rc.asf[1] * rc.baseASF[1] * rc.directions[1];// forward-backward
    qint8 z = rc.axes[2] * rc.asf[2] * rc.baseASF[2] * rc.directions[2];// up-down
    qint8 w = rc.axes[3] * rc.asf[3] * rc.baseASF[3] * rc.directions[3];// rotation
    qint8 d = rc.axes[4] * rc.asf[4] * rc.baseASF[4] * rc.directions[4];// pitch
    qint8 r = rc.axes[5] * rc.asf[5] * rc.baseASF[5] * rc.directions[5];// roll

    //TODO: directions and axes setup
    //Horizontal thrusters
    out->thrusterPower[0] = thrusterDirections[0] * (y+x+w);
    out->thrusterPower[1] = thrusterDirections[1] * (y-x-w);
    out->thrusterPower[2] = thrusterDirections[2] * (y-x+w);
    out->thrusterPower[3] = thrusterDirections[3] * (y+x-w);
    //Vertical thrusters
    out->thrusterPower[4] = thrusterDirections[4] * (z+d+r);
    out->thrusterPower[5] = thrusterDirections[5] * (z+d-r);
    out->thrusterPower[6] = thrusterDirections[6] * (z-d+r);
    out->thrusterPower[7] = thrusterDirections[7] * (z-d-r);

    QByteArray ba;

    QDataStream in(&ba, QIODevice::WriteOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);


    // begin v1
    in << out->header;
    in << out->version;
    in << out->auxFlags;
    for (int i = 0; i < 10; i++) {
        qint8 t = out->thrusterPower[i];
        in << t;
    }
    in << out->manipulator[0];
    in << out->manipulator[1];
    for (qint8 c : out->cameraRotation) {
        in << c;
    }

    in << out->camsel;

    emit controlReady(QByteArray(ba));
}

void RovDataParser::doProcessTelemetry(QByteArray datagram){
        RovTelemetry telemetry = RovTelemetry();

        if (quint8(datagram[0]) == RovTelemetry::header_telemetry) {
            QDataStream out(&datagram, QIODevice::ReadOnly);
            out.setFloatingPointPrecision(QDataStream::SinglePrecision);

            out >> telemetry.header;
            out >> telemetry.version;
            out >> telemetry.depth;
            out >> telemetry.pitch;
            out >> telemetry.yaw;
            out >> telemetry.roll;
            out >> telemetry.ammeter;
            out >> telemetry.voltmeter;
            out >> telemetry.cameraIndex;
            out >> telemetry.temperature;
//            qint16 crc = 0;
//            out >> crc;

//            qint16 currentCrc = calculateCRC(datagram.data(), datagram.size() - 2);

//            if (currentCrc != crc) {
//                telemetry.depth = 0.0f;
//                telemetry.pitch = 0.0f;
//                telemetry.yaw = 0.0f;
//                telemetry.roll = 0.0f;
//                telemetry.voltmeter = 0.0f;
//                telemetry.ammeter = 0.0f;
//                telemetry.cameraIndex = 0;
//            }

        }

        emit telemetryReady(telemetry);
}
