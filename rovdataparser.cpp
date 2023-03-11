#include "rovdataparser.h"
#include "helpers.h"

inline float constrain(float val, float min, float max){
    return val < min ? min : val > max ? max : val;
}

RovDataParser::RovDataParser(QObject *parent)
    : QObject{parent}
    , m_datagram(new RovDatagram())
    , m_thrOvrMutex()
    , m_thrOvrInvMutex()
    , m_overrideMutex()
{

}

void RovDataParser::doEnableThrustersOverride(bool state){
    m_overrideMutex.lock();
    this->m_override = state;
    m_overrideMutex.unlock();
}

void RovDataParser::doSetThrustersOverride(QList<qint8> override){
    m_thrOvrMutex.lock();
    for(int i = 0; i < 8; i++){

        qDebug() << override[i];
        this->m_thrOvr[i] = override.at(i);
    }
    m_thrOvrMutex.unlock();
}

void RovDataParser::doSetThrustersOverrideInvert(qint8 override){
    m_thrOvrInvMutex.lock();
    this->m_thrOvrInv = override;
    m_thrOvrInvMutex.unlock();
}

void RovDataParser::doPrepareDatagram(Joystick rc){
    m_overrideMutex.lock();
    bool state = m_override;
    m_overrideMutex.unlock();
//    qDebug() << QString::number(rc.buttons, 2);

    // Buttons processing
    m_datagram->manipulator[0] = BIT_CHECK(rc.buttons, 0) - BIT_CHECK(rc.buttons, 1);
    m_datagram->manipulator[1] = BIT_CHECK(rc.buttons, 2) - BIT_CHECK(rc.buttons, 3);
    m_datagram->camsel = BIT_CHECK(rc.buttons, 8);


    // Thrusters processing
    if(state){
        m_thrOvrMutex.lock();
        m_thrOvrInvMutex.lock();
        for(int i = 0; i < 8; i++){
            m_datagram->thrusterPower[i] = m_thrOvr[i] * (BIT_CHECK(m_thrOvrInv, i) ? -1:1);
        }
        m_thrOvrMutex.unlock();
        m_thrOvrInvMutex.unlock();
    }
    else{
        qint8 x = rc.axes[0] * rc.asf[0] * rc.baseASF[0] * rc.directions[0];// left-right
        qint8 y = rc.axes[1] * rc.asf[1] * rc.baseASF[1] * rc.directions[1];// forward-backward
        qint8 z = rc.axes[2] * rc.asf[2] * rc.baseASF[2] * rc.directions[2];// up-down
        qint8 w = rc.axes[3] * rc.asf[3] * rc.baseASF[3] * rc.directions[3];// rotation
        qint8 d = rc.axes[4] * rc.asf[4] * rc.baseASF[4] * rc.directions[4];// pitch
        qint8 r = rc.axes[5] * rc.asf[5] * rc.baseASF[5] * rc.directions[5];// roll

        //TODO: directions and axes setup
        //Horizontal thrusters
        m_datagram->thrusterPower[0] = constrain(thrusterDirections[0] * (x-y-w), -100, 100);
        m_datagram->thrusterPower[1] = constrain(thrusterDirections[1] * (x+y+w), -100, 100);
        m_datagram->thrusterPower[2] = constrain(thrusterDirections[2] * (x+y-w), -100, 100);
        m_datagram->thrusterPower[3] = constrain(thrusterDirections[3] * (x-y+w), -100, 100);
        //Vertical thrusters
        m_datagram->thrusterPower[4] = constrain(thrusterDirections[4] * (z+d+r), -100, 100);
        m_datagram->thrusterPower[5] = constrain(thrusterDirections[5] * (z+d-r), -100, 100);
        m_datagram->thrusterPower[6] = constrain(thrusterDirections[6] * (z-d+r), -100, 100);
        m_datagram->thrusterPower[7] = constrain(thrusterDirections[7] * (z-d-r), -100, 100);
    }
    // Hats processing
    m_datagram->cameraRotation[0] = rc.hats[0];
    m_datagram->cameraRotation[1] = rc.hats[1];



    int i = 0;
    qDebug() << QString(i) << ": " << m_datagram->thrusterPower[i++] << QString(i) << ": " << m_datagram->thrusterPower[i++] << QString(i) << ": " << m_datagram->thrusterPower[i++] << QString(i) << ": " << m_datagram->thrusterPower[i++] << QString(i) << ": " << m_datagram->thrusterPower[i++] << QString(i) << ": " << m_datagram->thrusterPower[i++] << QString(i) << ": " << m_datagram->thrusterPower[i++] << QString(i) << ": " << m_datagram->thrusterPower[i++];

    QByteArray ba;

    QDataStream in(&ba, QIODevice::WriteOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);


    // begin v1
    in << m_datagram->header;
    in << m_datagram->version;
    in << m_datagram->auxFlags;
    for (int i = 0; i < 10; i++) {
        qint8 t = m_datagram->thrusterPower[i];
        in << t;
    }
    in << m_datagram->manipulator[0];
    in << m_datagram->manipulator[1];
    for (qint8 c : m_datagram->cameraRotation) {
        in << c;
    }

    in << m_datagram->camsel;

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
//            qDebug() << telemetry.voltmeter << "V";
//            qDebug() << telemetry.ammeter << "A";

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
        else{
            qDebug() << "Wrong header";
        }

        emit telemetryProcessed(telemetry);
}
