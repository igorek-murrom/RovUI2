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
      m_auxControl(new RovAuxControl), m_auxControlMutex(), depthReg(25, 5, 0),
      yawReg(1, 10, 0), rollReg(5, 5, 0), pitchReg(3, 7, 0) {
    QTimer *auxTimer = new QTimer(this);
    connect(auxTimer, &QTimer::timeout, this,
            &RovDataParser::prepareAuxControl);
    auxTimer->start(64);
//    yawReg.enable();
//    depthReg.enable();
//    pitchReg.enable();
//    rollReg.enable();
}

void RovDataParser::enableThrustersOverride(bool state) {
    m_thrOvrEnableMutex.lock();
    m_thrOvrEnable = state;
    m_thrOvrEnableMutex.unlock();
}

void RovDataParser::setThrustersOverride(QList<qint8> override) {
    m_thrOvrMutex.lock();
    for (int i = 0; i < override.size(); i++) {

        // qDebug() << override[i];
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

void RovDataParser::setDepth(double val) { /*depthReg.setTarget(val);*/ }

void RovDataParser::setDepthStatus(int status) {
    if (status != 0)
        depthReg.enable();
    else
        depthReg.disable();
}

void RovDataParser::setYaw(double val) { /*yawReg.setTarget(val);*/ }

void RovDataParser::setYawStatus(int status) {
    if (status != 0)
        yawReg.enable();
    else
        yawReg.disable();
}

void RovDataParser::setRoll(double val) { /*rollReg.setTarget(val);*/ }

void RovDataParser::setRollStatus(int status) {
    if (status != 0)
        rollReg.enable();
    else
        rollReg.disable();
}

void RovDataParser::setPitch(double val) { /*pitchReg.setTarget(val);*/ }

void RovDataParser::setPitchStatus(int status) {
    if (status != 0)
        pitchReg.enable();
    else
        pitchReg.disable();
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

void RovDataParser::invalidateImuCalibration() {
    qDebug() << "decalibrating IMU" << Qt::endl;
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.imuInvCal = 1;
    m_auxControlMutex.unlock();
}

void RovDataParser::prepareAuxControl() {
    QByteArray  ba;
    QDataStream in(&ba, QIODevice::WriteOnly);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    m_auxControlMutex.lock();
    in << m_auxControl->header;
    in << m_auxControl->auxFlags.rawFlags;
    // in << m_auxControl->dDepth;
    // in << m_auxControl->dYaw;
    // in << m_auxControl->dRoll;
    // in << m_auxControl->dPitch;
    m_auxControl->auxFlags.imuInvCal = 0;
    m_auxControlMutex.unlock();
    emit auxControlReady(QByteArray(ba));
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float clamp180(float value) {
    if (value > 180) value -= 360;
    if (value < -180) value += 360;
    return value;
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
        (joy.buttons.ManipCCW - joy.buttons.ManipCW) * 100;
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
        int z;
        int x = joy.axes[0] * joy.runtimeASF[0] * joy.baseASF[0] *
                  joy.directions[0] *
                  (m_control->camsel == 1 ? -1 : 1); // left-right
        int y = -1 * joy.axes[1] * joy.runtimeASF[1] * joy.baseASF[1] *
                  joy.directions[1] *
                  (m_control->camsel == 1 ? -1 : 1); // forward-backward
        if (depthReg.enabled == 1)
        z = joy.axes[2] * /*joy.runtimeASF[2]*/  joy.baseASF[2] *
                  joy.directions[2]; // up-down
        else
        z = joy.axes[2] * joy.runtimeASF[2] * joy.baseASF[2] *
                joy.directions[2]; // up-down
        int w = -1*(joy.axes[3] * joy.runtimeASF[3] * joy.baseASF[3] *
                  joy.directions[3]) * 0.3;
        int d = joy.axes[4] */* joy.runtimeASF[4] **/ joy.baseASF[4] *
                  joy.directions[4];
        int r = joy.axes[5] /** joy.runtimeASF[5]*/ * joy.baseASF[5] *
                  joy.directions[5];

        rollReg.setTarget(rollEtalon);
        pitchReg.setTarget(pitchEtalon);
        if (abs(r) < 2) r += rollReg.eval(m_tele.roll);
        if (abs(d) < 2) d += pitchReg.eval(m_tele.pitch);

        // depthEtalon = constrain(depthEtalon += z / 1000.0, -1, 5);
        // depthReg.setTarget(depthEtalon);
        // z = depthReg.eval(m_tele.depth);

        // qDebug() << w;


        m_control->thrusterPower[2] = constrain(z + r - d, -100, 100);
        m_control->thrusterPower[3] = constrain(z + r + d, -100, 100);
        m_control->thrusterPower[5] = constrain(z - r + d, -100, 100);
        m_control->thrusterPower[6] = constrain(-z + r + d, -100, 100);

        m_control->thrusterPower[0] = constrain(x + y + w, -100, 100);
        m_control->thrusterPower[1] = constrain(-x + y +  w, -100, 100);
        m_control->thrusterPower[4] = constrain(x - y + w, -100, 100);
        m_control->thrusterPower[7] = constrain(x + y - w, -100, 100);

        QString a = "";
        for (auto i : m_control->thrusterPower) {
            a += QString::number(i);
            a += " ";
        }
        // qDebug() << a;
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

    m_tele = telemetry;

    if (isFirstParsing) {
        depthEtalon = m_tele.depth;
        isFirstParsing = false;
    }

    emit telemetryProcessed(telemetry);
}

float FPPDRegulator::eval(float data, bool isYaw) {
    if (!enabled) return 0;

    error = target - data;
    if (isYaw) error = (((int)error + 360) % 360) - 180;

    uP  = kP * error;                                           // kP(error)
    uD  = kD * ((error - lastError) / (eTimer.nsecsElapsed())); // kD(de/dt)
    uI += kI * error * eTimer.nsecsElapsed();

    lastError = error;
    eTimer.start();
    lastData = data;
    float u = uP + uD;
//    qDebug() << "error:" << err << "  data:" << data << "   target: " << target << "  kP:" << kP << "  kD:" << kD << "  uP:" << uP << "  uD:" << uD << "  u:" << u;

    return u > 127.0f ? 127.0f : u < -128.0f ? -128.0f : u;
}

void FPPDRegulator::enable() {
    lastData  = 0;
    lastError = 0;
    eTimer.start();
    enabled = 1;
}
void FPPDRegulator::disable() { enabled = 0; }
void FPPDRegulator::setTarget(float target) { this->target = target; }
