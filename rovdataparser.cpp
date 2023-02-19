#include "rovdataparser.h"

RovDataParser::RovDataParser(QObject *parent)
    : QObject{parent}
{

}


void RovDataParser::doPrepareDatagram(RovControl rc){
    RovDatagram out = RovDatagram();
    qint8 x = rc.axes[0] * rc.asf[0];// left-right
    qint8 y = rc.axes[1] * rc.asf[1];// forward-backward
    qint8 z = rc.axes[2] * rc.asf[2];// up-down
    qint8 w = rc.axes[3] * rc.asf[3];// rotation
    qint8 d = rc.axes[4] * rc.asf[4];// pitch
    qint8 r = rc.axes[5] * rc.asf[5];// roll

    //TODO: directions and axes setup
    //Horizontal thrusters
    out.thrusterPower[0] = rc.directions[0] * (y+x+w);
    out.thrusterPower[1] = rc.directions[1] * (y-x-w);
    out.thrusterPower[2] = rc.directions[2] * (y-x+w);
    out.thrusterPower[3] = rc.directions[3] * (y+x-w);
    //Vertical thrusters
    out.thrusterPower[4] = rc.directions[4] * (z+d+r);
    out.thrusterPower[5] = rc.directions[5] * (z+d-r);
    out.thrusterPower[6] = rc.directions[6] * (z-d+r);
    out.thrusterPower[7] = rc.directions[7] * (z-d-r);
    emit datagramReady(RovDatagram(out));
}

void RovDataParser::doUpdateTelemetry(QByteArray datagram){
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
            qint16 crc = 0;
            out >> crc;

            qint16 currentCrc = calculateCRC(datagram.data(), datagram.size() - 2);

            if (currentCrc != crc) {
                telemetry.depth = 0.0f;
                telemetry.pitch = 0.0f;
                telemetry.yaw = 0.0f;
                telemetry.roll = 0.0f;
                telemetry.voltmeter = 0.0f;
                telemetry.ammeter = 0.0f;
                telemetry.cameraIndex = 0;
            }

        }

        emit telemetryUpdated();
}
