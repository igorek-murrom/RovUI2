#include "rovdataparser.h"
#include "ui_dataparser.h"

#define stringify_please (x) #x

inline float constrain(float val, float min, float max) {
    return val < min ? min : val > max ? max : val;
}
inline float wFunction(float value) {
    int sign = value < 0 ? 1: -1;
    return value;
    // return sign * 0.005 * value * value;
}
RovDataParser::RovDataParser(QWidget *parent)
    : QDialog{parent}, ui(new Ui::DataParser), m_control(new RovControl()),
      m_controlMutex(), overrideTelemetryUpdate(new QTimer(this)),
      m_auxControl(new RovAuxControl), m_auxControlMutex(),
      depthReg(200, 0.5, 50),
      yawReg(1, 0, 1),
      rollReg(0.5, 0, 0),
      pitchReg(0.5, 0, 0) {
    ui->setupUi(this);

    QTimer *auxTimer = new QTimer(this);
    connect(auxTimer, &QTimer::timeout, this,
            &RovDataParser::prepareAuxControl);
    auxTimer->start(64);
    connect(overrideTelemetryUpdate, &QTimer::timeout, this, [this]() {
        processTelemetry(QByteArray());
        prepareControl(Joystick());
    });
    connect(ui->overrideTelemetry, &QCheckBox::clicked, this,
            [this](bool state) {
                if (state) {
                    overrideTelemetryUpdate->start(16);
                } else {
                    overrideTelemetryUpdate->stop();
                }
            });

    connect(this, SIGNAL(controlReady(QByteArray)), this, SLOT(prepareUpdateServo()));
}

void RovDataParser::corallAuto() {
    isCorall = !isCorall;
}

void RovDataParser::setAuxFlags(qint8 val) {
    m_auxControlMutex.lock();
    m_auxControl->auxFlags.rawFlags = val;
    m_auxControlMutex.unlock();
}

void RovDataParser::setDepth(double val) { depthReg.setTarget(val); }

void RovDataParser::setDepthStatus(int status) {
    if (status != 0)
        depthReg.enable();
    else
        depthReg.disable();
}

void RovDataParser::setYaw(double val) { yawReg.setTarget(val); }

void RovDataParser::setYawStatus(int status) {
    if (status != 0)
        yawReg.enable();
    else
        yawReg.disable();
}

void RovDataParser::setRoll(double val) { rollReg.setTarget(val); }

void RovDataParser::setRollStatus(int status) {
    if (status != 0)
        rollReg.enable();
    else
        rollReg.disable();
}

void RovDataParser::setPitch(double val) { pitchReg.setTarget(val); }

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

void RovDataParser::prepareUpdateServo() {
    m_digitServoPos += 3 * m_control.data()->cameraRotationDelta[0];
    m_digitServoPos = constrain(m_digitServoPos, -100, 100);
    emit servoDigitReady(m_digitServoPos);
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

void RovDataParser::prepareControl(Joystick joy) {
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
    if (ui->overrideControl->checkState() == Qt::Checked) {
        m_control->thrusterPower[0] = ui->thrusterSpinbox1->value();
        m_control->thrusterPower[1] = ui->thrusterSpinbox2->value();
        m_control->thrusterPower[2] = ui->thrusterSpinbox3->value();
        m_control->thrusterPower[3] = ui->thrusterSpinbox4->value();
        m_control->thrusterPower[4] = ui->thrusterSpinbox5->value();
        m_control->thrusterPower[5] = ui->thrusterSpinbox6->value();
        m_control->thrusterPower[6] = ui->thrusterSpinbox7->value();
        m_control->thrusterPower[7] = ui->thrusterSpinbox8->value();
    }
    // else if (isCorall) {
    //     depthReg.enable();
    //     m_digitServoPos = 0;
    //     x = 0;
    //     y = 0;
    //     z = 0;
    //     w = 0;
    //     d = 0;
    //     r = 0;

    //     depthReg.setTarget(3);
    //     float dReg  = depthReg.eval(m_tele.depth);
    //     z          -= dReg;

    //            // vertical
    //     m_control->thrusterPower[2] = constrain(z + r - d, -100, 100);
    //     m_control->thrusterPower[3] = constrain(z + r + d, -100, 100);
    //     m_control->thrusterPower[5] = constrain(z - r + d, -100, 100);
    //     m_control->thrusterPower[6] = constrain(z - r - d, -100, 100); // swap

    //            // horizontal
    //     m_control->thrusterPower[0] = constrain(x - y + w, -100, 100); // swap
    //     m_control->thrusterPower[1] = constrain(x + y - w, -100, 100); // swap
    //     m_control->thrusterPower[4] = constrain(x + y + w, -100, 100);
    //     m_control->thrusterPower[7] = constrain(x - y - w, -100, 100);

    //     m_control->thrusterPower[0] *= -1;
    //     m_control->thrusterPower[1] *= -1;
    //     m_control->thrusterPower[6] *= -1;

    //     if (m_control->thrusterPower[1] < 0) m_control->thrusterPower[1] = constrain(m_control->thrusterPower[1] - 4, -100, 100);
    //     if (m_control->thrusterPower[7] > 0) m_control->thrusterPower[7] = constrain(m_control->thrusterPower[7] + 4, -100, 100);


    //     ui->thrusterSpinbox1->setValue(m_control->thrusterPower[0]);
    //     ui->thrusterSpinbox2->setValue(m_control->thrusterPower[1]);
    //     ui->thrusterSpinbox3->setValue(m_control->thrusterPower[2]);
    //     ui->thrusterSpinbox4->setValue(m_control->thrusterPower[3]);
    //     ui->thrusterSpinbox5->setValue(m_control->thrusterPower[4]);
    //     ui->thrusterSpinbox6->setValue(m_control->thrusterPower[5]);
    //     ui->thrusterSpinbox7->setValue(m_control->thrusterPower[6]);
    //     ui->thrusterSpinbox8->setValue(m_control->thrusterPower[7]);
    //     ui->progressBarX->setValue(x);
    //     ui->progressBarY->setValue(y);
    //     ui->progressBarZ->setValue(z);
    //     ui->progressBarW->setValue(w);
    //     ui->progressBarD->setValue(d);
    //     ui->progressBarR->setValue(r);

    //     depthReg.disable();
    // }
    else {
        //mainASF = constrain(((joy.axis[6].axe / 100.0) + 1) / 2, 0, 1);
        mainASF = 1;
        x = joy.axis[0].axe * joy.axis[0].runtimeASF * joy.axis[0].baseASF * joy.axis[0].direction * mainASF * (m_control->camsel == 1 ? -1 : 1);
        y = joy.axis[1].axe * joy.axis[1].runtimeASF * joy.axis[1].baseASF * joy.axis[1].direction * mainASF * (m_control->camsel == 1 ? -1 : 1);
        z = joy.axis[2].axe/* * joy.axis[2].runtimeASF*/ * joy.axis[2].baseASF * joy.axis[2].direction * mainASF;
        w = joy.axis[3].axe * joy.axis[3].runtimeASF * joy.axis[3].baseASF * joy.axis[3].direction;
        d = joy.axis[4].axe * joy.axis[4].runtimeASF * joy.axis[4].baseASF * joy.axis[4].direction * mainASF;
        r = joy.axis[5].axe * joy.axis[5].runtimeASF * joy.axis[5].baseASF * joy.axis[5].direction * mainASF;

        // fuck the z axe
        if (!-joy.buttons.AddButton) z *= 0.25;


        // dead zones
        if (abs(x) < 5) x = 0;
        // else x = (x < 0 ? -1 : 1) * (abs(x) - 1);

        if (abs(y) < 6) y = 0;
        // else y = (y < 0 ? -1 : 1) * (abs(y) - 2);

        if (abs(z) < 6) z = 0;
        // else z = (z < 0 ? -1 : 1) * (abs(z) - 2);

        if (abs(r) < 7) r = 0;
        if (abs(d) < 7) d = 0;


        // fix w axe
        w = wFunction(w) * -0.25;
        // w += 1;
        // w += (int)joy.axis[6].axe / 25;

        if (isCorall) {
            m_digitServoPos = 0;
            depthReg.enable();
            depthReg.setTarget(3);
        }

        // regulator compensate
        float yReg  = yawReg.eval(m_tele.yaw > 0 ? m_tele.yaw : m_tele.yaw + 360, true);
        float dReg  = depthReg.eval(m_tele.depth);
        float rReg  = rollReg.eval(m_tele.roll);
        float pReg  = pitchReg.eval(m_tele.pitch);
        w          += yReg;
        z          -= dReg;
        r          -= rReg;
        d          += pReg;

        // vertical
        m_control->thrusterPower[2] = constrain(z + r - d, -100, 100);
        m_control->thrusterPower[3] = constrain(z + r + d, -100, 100);
        m_control->thrusterPower[5] = constrain(z - r + d, -100, 100);
        m_control->thrusterPower[6] = constrain(z - r - d, -100, 100); // swap

        // horizontal
        m_control->thrusterPower[0] = constrain(x - y + w, -100, 100); // swap
        m_control->thrusterPower[1] = constrain(x + y - w, -100, 100); // swap
        m_control->thrusterPower[4] = constrain(x + y + w, -100, 100);
        m_control->thrusterPower[7] = constrain(x - y - w, -100, 100);

        // fix rear direction
        // for (int8_t thruster : m_control->thrusterPower) thruster = thruster < 0 ? constrain(thruster + 3, -100, 100) : thruster;

        // fix direction
        m_control->thrusterPower[0] *= -1;
        m_control->thrusterPower[1] *= -1;
        m_control->thrusterPower[6] *= -1;

        // if (m_control->thrusterPower[1] < 0) m_control->thrusterPower[1] = constrain(m_control->thrusterPower[1] - 4, -100, 100);
        // if (m_control->thrusterPower[7] > 0) m_control->thrusterPower[7] = constrain(m_control->thrusterPower[7] + 4, -100, 100);


        ui->thrusterSpinbox1->setValue(m_control->thrusterPower[0]);
        ui->thrusterSpinbox2->setValue(m_control->thrusterPower[1]);
        ui->thrusterSpinbox3->setValue(m_control->thrusterPower[2]);
        ui->thrusterSpinbox4->setValue(m_control->thrusterPower[3]);
        ui->thrusterSpinbox5->setValue(m_control->thrusterPower[4]);
        ui->thrusterSpinbox6->setValue(m_control->thrusterPower[5]);
        ui->thrusterSpinbox7->setValue(m_control->thrusterPower[6]);
        ui->thrusterSpinbox8->setValue(m_control->thrusterPower[7]);
        ui->progressBarX->setValue(x);
        ui->progressBarY->setValue(y);
        ui->progressBarZ->setValue(z);
        ui->progressBarW->setValue(w);
        ui->progressBarD->setValue(d);
        ui->progressBarR->setValue(r);

        if (isCorall) {
            depthReg.disable();
        }
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
    // qDebug() << m_control->manipulatorOpenClose << m_control->manipulatorRotate;
    in << m_control->camsel;
    m_controlMutex.unlock();

    emit controlReady(QByteArray(ba));
}

void RovDataParser::processTelemetry(QByteArray datagram) {
    RovTelemetry telemetry = RovTelemetry();
    if (ui->overrideTelemetry->checkState() == Qt::Checked) {
        telemetry.current = ui->curSB->value();
        telemetry.voltage = ui->voltSB->value();
        telemetry.depth   = ui->depthSB->value();
        telemetry.yaw     = ui->yawSB->value();
        telemetry.roll    = ui->rollSB->value();
        telemetry.pitch   = ui->pitchSB->value();
        m_tele            = telemetry;
        emit telemetryProcessed(telemetry);
    } else {
        char buffer[datagram.size()];
        memcpy(buffer, datagram.data(), datagram.size());
        size_t i = 0;

        helpers::read_bytes(buffer, i, telemetry.version);
        helpers::read_bytes(buffer, i, telemetry.depth);
        helpers::read_bytes(buffer, i, telemetry.pitch);
        helpers::read_bytes(buffer, i, telemetry.yaw);
        helpers::read_bytes(buffer, i, telemetry.roll);
        helpers::read_bytes(buffer, i, telemetry.current);
        helpers::read_bytes(buffer, i, telemetry.voltage);
        helpers::read_bytes(buffer, i, telemetry.cameraIndex);
        helpers::read_bytes(buffer, i, telemetry.temp);

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

        telemetry.yaw *= -1;
        m_tele = telemetry;

        ui->depthSB->setValue(telemetry.depth);
        ui->yawSB->setValue(telemetry.yaw);
        ui->rollSB->setValue(telemetry.roll);
        ui->pitchSB->setValue(telemetry.pitch);
        ui->voltSB->setValue(telemetry.voltage);
        ui->curSB->setValue(telemetry.current);
        emit telemetryProcessed(telemetry);
    }
}

float FPPDRegulator::eval(float data, bool yawFlag) {
    if (!enabled)
        return 0;

    error  = target - data;
    if (yawFlag) {
        error = ((int)error + 180) % 360;
        if (error < 0) error += 180;
        else error -= 180;
    }

    uP = kP * error;
    uD = kD * ((error - lastError)/* / eTimer.elapsed()*/);
    uI += kI * error/* * float(eTimer.elapsed())*/;
    if (abs(error) < 1) uI = 0;
    // lastError = isnan(error) ? 0 : error;
    eTimer.restart();
    // eTimer.start();
    lastData = data;

    float u = uP + uD + uI;
    // qDebug() << eTimer.elapsed();
    return constrain(u, -100, 100);
}

void FPPDRegulator::enable() {
    lastData  = 0;
    lastError = 0;
    uI        = 0;
    eTimer.start();
    enabled = 1;
}
void FPPDRegulator::disable() { enabled = 0; }
void FPPDRegulator::setTarget(float target) { this->target = target; }
