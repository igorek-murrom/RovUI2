#include "rovdataparser.h"
#include "helpers.h"

inline float constrain(float val, float min, float max){
    return val < min ? min : val > max ? max : val;
}

RovDataParser::RovDataParser(QObject *parent)
    : QObject{parent}
    , m_thrOvrMutex()
    , m_thrOvrInvMutex()
    , m_thrOvrEnable()
    , m_thrOvrEnableMutex()
    , m_datagram(new RovControlDatagram())
    , m_datagramMutex()
    , m_auxDatagram(new RovAuxDatagram)
    , m_auxDatagramMutex()
{
}

void RovDataParser::enableThrustersOverride(bool state){
    m_thrOvrEnableMutex.lock();
    m_thrOvrEnable = state;
    m_thrOvrEnableMutex.unlock();
}

void RovDataParser::setThrustersOverride(QList<qint8> override){
    m_thrOvrMutex.lock();
    for(int i = 0; i < override.size(); i++){

        qDebug() << override[i];
        this->m_thrOvr[i] = override.at(i);
    }
    m_thrOvrMutex.unlock();
}

void RovDataParser::setThrustersOverrideInvert(qint8 override){
    m_thrOvrInvMutex.lock();
    this->m_thrOvrInv = override;
    m_thrOvrInvMutex.unlock();
}

void RovDataParser::setAuxFlags(qint8 val){
    m_auxDatagramMutex.lock();
    m_auxDatagram->auxFlags = val;
    m_auxDatagramMutex.unlock();
}

void RovDataParser::setDepth(double val){
    m_auxDatagramMutex.lock();
    m_auxDatagram->dDepth = val;
    m_auxDatagramMutex.unlock();
    prepareAuxControl();
}
void RovDataParser::setYaw(double val){
    m_auxDatagramMutex.lock();
    m_auxDatagram->dYaw = val;
    m_auxDatagramMutex.unlock();
    prepareAuxControl();
}
void RovDataParser::setRoll(double val){
    m_auxDatagramMutex.lock();
    m_auxDatagram->dRoll = val;
    m_auxDatagramMutex.unlock();
    prepareAuxControl();
}
void RovDataParser::setPitch(double val){
    m_auxDatagramMutex.lock();
    m_auxDatagram->dPitch = val;
    m_auxDatagramMutex.unlock();
    prepareAuxControl();
}

void RovDataParser::prepareAuxControl(){
    QByteArray ba;

    QDataStream in(&ba, QIODevice::WriteOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    m_auxDatagramMutex.lock();
    in << m_auxDatagram->header;
    in << m_auxDatagram->auxFlags;
    in << m_auxDatagram->dDepth;
    in << m_auxDatagram->dYaw;
    in << m_auxDatagram->dRoll;
    in << m_auxDatagram->dPitch;
    m_auxDatagramMutex.unlock();
    emit auxControlReady(QByteArray(ba));
}

void RovDataParser::prepareControl(Joystick rc){
    m_thrOvrEnableMutex.lock();
    bool state = m_thrOvrEnable;
    m_thrOvrEnableMutex.unlock();

    m_datagramMutex.lock();
    // Buttons processing
    m_datagram->manipulator[0] = BIT_CHECK(rc.buttons, 0) - BIT_CHECK(rc.buttons, 1); // 0th is open, 1st is close
    m_datagram->manipulator[1] = BIT_CHECK(rc.buttons, 2) - BIT_CHECK(rc.buttons, 3); // 2nd is CCW, 3rd is CW
    if(m_prevCamSelState == 0 && BIT_CHECK(rc.buttons, 8) == 1)
        m_datagram->camsel = !m_datagram->camsel;
    m_prevCamSelState = BIT_CHECK(rc.buttons, 8);


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
    m_datagram->cameraRotation[0] = rc.hats[1];
    m_datagram->cameraRotation[1] = rc.hats[0];

    QByteArray ba;

    QDataStream in(&ba, QIODevice::WriteOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);


    // begin v1
    in << m_datagram->header;
    in << m_datagram->version;
//    in << m_datagram->auxFlags;
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
    m_datagramMutex.unlock();
    emit controlReady(QByteArray(ba));
}

void RovDataParser::processTelemetry(QByteArray datagram){
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
            out >> telemetry.current;
            out >> telemetry.voltmeter;
            out >> telemetry.cameraIndex;
            out >> telemetry.temperature;

        }
        else{
            qDebug() << "Wrong header";
        }

        emit telemetryProcessed(telemetry);
}
