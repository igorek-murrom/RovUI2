#include "rovdataparser.h"
#include "helpers.h"
#include "qdatetime.h"
#include "qtimer.h"
#include <cstddef>
#include <cstring>

inline float constrain(float val, float min, float max) {
    return val < min ? min : val > max ? max : val;
}

RovDataParser::RovDataParser(QWidget *parent)
    : QWidget{parent}, m_thrOvrMutex(), m_thrOvrInvMutex(), m_thrOvrEnable(),
      m_thrOvrEnableMutex(), m_control(new RovControl()), m_controlMutex(),
      m_auxControl(new RovAuxControl), m_auxControlMutex() {
    QTimer *auxTimer = new QTimer(this);
    connect(auxTimer, &QTimer::timeout, this,
            &RovDataParser::prepareAuxControl);
    auxTimer->start(64);
}

void RovDataParser::enableThrustersOverride(bool state) {
    m_thrOvrEnableMutex.lock();
    m_thrOvrEnable = state;
    m_thrOvrEnableMutex.unlock();
}

void RovDataParser::setThrustersOverride(QList<qint8> override) {
    m_thrOvrMutex.lock();
    for (int i = 0; i < override.size(); i++) {

        qDebug() << override[i];
        this->m_thrOvr[i] = override.at(i);
    }
    m_thrOvrMutex.unlock();
}

void RovDataParser::setThrustersOverrideInvert(qint8 override) {
    m_thrOvrInvMutex.lock();
    this->m_thrOvrInv = override;
    m_thrOvrInvMutex.unlock();
}

void RovDataParser::setAuxFlags(qint8 val) {
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.rawFlags = val;
    m_auxControlMutex.unlock();
}

void RovDataParser::setDepth(double val) {
    m_auxControlMutex.lock();
    m_auxControl->dDepth = val;
    m_auxControlMutex.unlock();
}

void RovDataParser::setDepthStatus(int status) {
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.eDepth = status != 0;
    m_auxControlMutex.unlock();
}

void RovDataParser::setYaw(double val) {
    m_auxControlMutex.lock();
    m_auxControl->dYaw = val;
    m_auxControlMutex.unlock();
}

void RovDataParser::setYawStatus(int status) {
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.eYaw = status != 0;
    m_auxControlMutex.unlock();
}

void RovDataParser::setRoll(double val) {
    m_auxControlMutex.lock();
    m_auxControl->dRoll = val;
    m_auxControlMutex.unlock();
}

void RovDataParser::setRollStatus(int status) {
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.eRoll = status != 0;
    m_auxControlMutex.unlock();
}

void RovDataParser::setPitch(double val) {
    m_auxControlMutex.lock();
    m_auxControl->dPitch = val;
    m_auxControlMutex.unlock();
}

void RovDataParser::setPitchStatus(int status) {
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.ePitch = status != 0;
    m_auxControlMutex.unlock();
}

void RovDataParser::toggleLight() {
    qDebug() << "toggling light" << Qt::endl;
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.eLight = !m_auxControl->auxFlags.eLight;
    m_auxControlMutex.unlock();
}

void RovDataParser::togglePump() {
    qDebug() << "toggling anal pump" << Qt::endl;
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.ePump = !m_auxControl->auxFlags.ePump;
    m_auxControlMutex.unlock();
}

void RovDataParser::prepareAuxControl() {
    QByteArray  ba;
    QDataStream in(&ba, QIODevice::WriteOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    m_auxControlMutex.lock();
    in << m_auxControl->header;
    in << m_auxControl->auxFlags.rawFlags;
    in << m_auxControl->dDepth;
    in << m_auxControl->dYaw;
    in << m_auxControl->dRoll;
    in << m_auxControl->dPitch;
    m_auxControlMutex.unlock();
    emit auxControlReady(QByteArray(ba));
}

void RovDataParser::prepareControl(Joystick joy) {
    m_thrOvrEnableMutex.lock();
    bool state = m_thrOvrEnable;
    m_thrOvrEnableMutex.unlock();

    m_controlMutex.lock();
    // Buttons processing
    m_control->manipulatorOpenClose =
        (joy.buttons.ManipOpen - joy.buttons.ManipClose) * 100;
    m_control->manipulatorRotate =
        (joy.buttons.ManipCCW - joy.buttons.ManipCW) * 60;
    if (m_prevCamSelState == 0 && joy.buttons.CameraSelect != 0)
        m_control->camsel = !m_control->camsel;
    m_prevCamSelState = joy.buttons.CameraSelect;

    // Thrusters processing
    if (state) {
        m_thrOvrMutex.lock();
        m_thrOvrInvMutex.lock();
        for (int i = 0; i < 8; i++) {
            m_control->thrusterPower[i] =
                m_thrOvr[i] * (BIT_CHECK(m_thrOvrInv, i) ? -1 : 1);
        }
        m_thrOvrMutex.unlock();
        m_thrOvrInvMutex.unlock();
    } else {
        // clang-format off
        qint8 x = joy.axes[0] * joy.runtimeASF[0] * joy.baseASF[0] *
                  joy.directions[0] * (m_control->camsel == 1 ? -1 : 1); // left-right
        qint8 y = joy.axes[1] * joy.runtimeASF[1] * joy.baseASF[1] *
                  joy.directions[1] * (m_control->camsel == 1 ? -1 : 1); // forward-backward
        // clang-format on
        qint8 z =
            joy.axes[2] * 1 * joy.baseASF[2] * joy.directions[2]; // up-down
        qint8 w = joy.axes[3] * joy.runtimeASF[3] * joy.baseASF[3] *
                  joy.directions[3]; // rotation
        qint8 d = joy.axes[4] * joy.runtimeASF[4] * joy.baseASF[4] *
                  joy.directions[4]; // pitch
        qint8 r = joy.axes[5] * joy.runtimeASF[5] * joy.baseASF[5] *
                  joy.directions[5]; // roll

        // enum thrusters {
        //     lo_fr_le = 9,
        //     lo_fr_ri = 3,
        //     hi_fr_le = 4,
        //     hi_fr_ri = 2,
        //     lo_ba_le = 7,
        //     lo_ba_ri = 5,
        //     hi_ba_le = 8,
        //     hi_ba_ri = 6,
        // };

        // TODO: directions and axes setup
        // Horizontal thrusters
        m_control->thrusterPower[0] =
            constrain(-x + y + z - w - d + r, -100, 100); // lfl
        m_control->thrusterPower[1] =
            constrain(-x - y + z + w - d - r, -100, 100); // lfr
        m_control->thrusterPower[2] =
            constrain(-x + y - z - w + d - r, -100, 100); // hfl
        m_control->thrusterPower[3] =
            constrain(-x - y - z + w + d + r, -100, 100); // hfr
        // Vertical thrusters
        m_control->thrusterPower[4] =
            constrain(-x - y - z - w - d - r, -100, 100); // lbl
        m_control->thrusterPower[5] =
            constrain(-x + y - z + w - d + r, -100, 100); // lbr
        m_control->thrusterPower[6] =
            constrain(-x - y + z - w + d + r, -100, 100); // hbl
        m_control->thrusterPower[7] =
            constrain(-x + y + z + w + d - r, -100, 100); // hbr
    }
    // Hats processing
    m_control->cameraRotationDelta[0] = joy.hats[1];
    m_control->cameraRotationDelta[1] = -joy.hats[0];

    QByteArray ba;

    QDataStream in(&ba, QIODevice::WriteOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // begin v1
    in << m_control->header;
    in << m_control->version;
    //    in << m_datagram->auxFlags;
    for (int i = 0; i < 10; i++) {
        qint8 t = m_control->thrusterPower[i];
        in << t;
    }
    in << m_control->manipulatorOpenClose;
    in << m_control->manipulatorRotate;
    for (qint8 c : m_control->cameraRotationDelta) {
        in << c;
    }

    in << m_control->camsel;
    m_controlMutex.unlock();
    emit controlReady(QByteArray(ba));
}

void RovDataParser::processTelemetry(QByteArray datagram) {
    RovTelemetry telemetry = RovTelemetry();
    char         buffer[datagram.size() + 1];
    memcpy(buffer, datagram.data(), datagram.size());
    size_t i = 0;

    //    helpers::read_bytes(buffer, i, telemetry.version);
    //    helpers::read_bytes(buffer, i, telemetry.depth);
    //    helpers::read_bytes(buffer, i, telemetry.pitch);
    //    helpers::read_bytes(buffer, i, telemetry.yaw);
    //    helpers::read_bytes(buffer, i, telemetry.roll);
    //    helpers::read_bytes(buffer, i, telemetry.current);
    //    helpers::read_bytes(buffer, i, telemetry.voltage);
    //    helpers::read_bytes(buffer, i, telemetry.cameraIndex);
    //    helpers::read_bytes(buffer, i, telemetry.temp);

    QDataStream out(&datagram, QIODevice::ReadOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    qint8 header = 0x00;
    out >> header;

    out >> telemetry.version;
    out >> telemetry.depth;
    out >> telemetry.pitch;
    out >> telemetry.yaw;
    out >> telemetry.roll;
    out >> telemetry.current;
    out >> telemetry.voltage;
    out >> telemetry.cameraIndex;
    out >> telemetry.temp;

    emit telemetryProcessed(telemetry);
}
