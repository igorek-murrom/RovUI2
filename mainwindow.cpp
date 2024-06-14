#include "mainwindow.h"
#include "ui_mainwindow2.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      m_cameraCapture(new RovCameraCapture(this)),
      m_joystickHandler(new JoystickHandler(this)),
      m_jsd(new JoystickSetupDialog(this)),
      m_rovCommunication(new RovCommunication(this)),
      m_rovDataParser(new RovDataParser(this)),
      m_datasplines(new RovDataSplines(this)),
      m_rovStatusLabel(new QLabel(this)),
      m_rovStatusIndicator(new LEDIndicator(this)),
      m_rovCameraCommunication(new RovCameraCommunication(this)),
      m_cameraSettings(new CameraSettings(new QDialog(this))),
      m_compassWidget(new InstrumentWidget("Compass", this)),
      m_gyroWidget(new InstrumentWidget("AttitudeIndicator", this)) {

    ui->setupUi(this);
    setWindowTitle("RovUI2 v1.0");
    ui->viewfinder->show();
    m_cameraCapture->setViewfinder(ui->viewfinder);
    m_compassWidget->transaction();
    m_compassWidget->updateView(0, 0, 0);
    m_gyroWidget->transaction();
    m_compassWidget->updateView(0, 0, 0);
    ui->compassLayout->addWidget(m_compassWidget.data());
    ui->compassLayout->addWidget(m_gyroWidget.data());

    setupStatusbar();
    updateStatusbarText("Initialization...");

    createConnections();
    updateTelemetry(RovTelemetry());

    QTimer *statusbarIndicatorsTimer = new QTimer(this);
    connect(statusbarIndicatorsTimer, &QTimer::timeout, this,
            &MainWindow::updateStatusbarIndicators);

    updateStatusbarText("RovUI2 initialization complete ^_^");
    updateStatusbarProgress(100);

    ui->yawSpinBox->setRange(0, 360);
}

MainWindow::~MainWindow() {
    m_cameraCapture.data()->stopCapture();
    delete ui;
}

void MainWindow::updateStatusbar() {
    //    ui->camLabel->setPixmap(QPixmap::fromImage(img));
    // updateStatusbarText(m_cameraCapture->getRecordInfo());
}

void MainWindow::updateDatasplines(RovTelemetry tele) {
    m_datasplines.data()->addVoltageSample(QPointF(m_vSamples++, tele.voltage));
    m_datasplines.data()->addCurrentSample(QPointF(m_aSamples++, tele.current));
    m_datasplines.data()->addYawSample(QPointF(m_ySamples++, tele.yaw));
    m_datasplines.data()->addDepthSample(QPointF(m_dSamples++, tele.depth));
    m_datasplines.data()->addPitchSample(QPointF(m_pSamples++, tele.pitch));
    m_datasplines.data()->addRollSample(QPointF(m_rSamples++, tele.roll));

}

void MainWindow::updateStatusbarText(QString status) {
    ui->statusbar->showMessage(status, 0);
}

void MainWindow::updateStatusbarProgress(int progress) {
    QProgressBar *pb = new QProgressBar(ui->statusbar);
    pb->setValue(progress);
    ui->statusbar->addWidget(pb);
}

void MainWindow::updateStatusbarIndicators() {}

void MainWindow::setDesiredDepth(float val) { ui->depthSpinBox->setValue(val); }
void MainWindow::setDesiredYaw(float val) { ui->yawSpinBox->setValue(val); }
void MainWindow::setDesiredRoll(float val) { ui->rollSpinBox->setValue(val); }
void MainWindow::setDesiredPitch(float val) { ui->pitchSpinBox->setValue(val); }

void MainWindow::overrideDepth(bool readonly) {
    ui->depthSpinBox->setReadOnly(readonly);
    ui->depthSpinBox->setEnabled(!readonly);
    ui->depthRegulatorCB->setCheckable(!readonly);
    ui->depthRegulatorCB->setEnabled(!readonly);
}
void MainWindow::overrideYaw(bool readonly) {
    ui->yawSpinBox->setReadOnly(readonly);
    ui->yawSpinBox->setEnabled(!readonly);
    ui->yawRegulatorCB->setCheckable(!readonly);
    ui->yawRegulatorCB->setEnabled(!readonly);
}
void MainWindow::overrideRoll(bool readonly) {
    ui->rollSpinBox->setReadOnly(readonly);
    ui->rollSpinBox->setEnabled(!readonly);
    ui->rollRegulatorCB->setCheckable(!readonly);
    ui->rollRegulatorCB->setEnabled(!readonly);
}
void MainWindow::overridePitch(bool readonly) {
    ui->pitchSpinBox->setReadOnly(readonly);
    ui->pitchSpinBox->setEnabled(!readonly);
    ui->pitchRegulatorCB->setCheckable(!readonly);
    ui->pitchRegulatorCB->setEnabled(!readonly);
}

void MainWindow::updateTelemetry(RovTelemetry tele) {
    ui->teleVersionLabel->setText(QString::number(tele.version));
    if (tele.depth < 3.4E+38)
        ui->teleDepthLabel->setText(QString::number(tele.depth, 'g', 2) +
                                    QString(lastDepth < tele.depth   ? " m ↑"
                                            : lastDepth > tele.depth ? " m ↓"
                                                                     : " m ="));
    else
        ui->teleDepthLabel->setText(QString("[DIS]"));
    lastDepth = tele.depth;
    ui->teleTempLabel->setText(
        tele.depth < 3.4E+38 ? QString::number(tele.temp / 100, 'f', 2) + " °C"
                             : "[DIS]");
    ui->teleVoltageLabel->setText(QString::number(tele.voltage, 'f', 2) + " V");
    ui->teleCurrentLabel->setText(
        QString::number(std::min(tele.current * 1000, 25000.0f), 'f', 2) +
        " mA");
    ui->teleCamSelLabel->setText(
        QString((tele.cameraIndex == 0 ? "Front" : "Rear")));
    lastTele = tele;
    m_compassWidget->updateView(tele.yaw, tele.roll, tele.pitch);
    m_gyroWidget->updateView(tele.yaw, tele.roll, tele.pitch);
}

void MainWindow::updateASF(float factor) { emit asfUpdated(factor); }

void MainWindow::createConnections() {
    // Camera
    connect(ui->actionBegin_camera_capture, &QAction::triggered, this,
            [this] { m_cameraCapture.data()->setSource(); m_cameraCapture.data()->startCapture(); });
    connect(ui->actionEnd_camera_capture, &QAction::triggered, this,
            [this] { m_cameraCapture.data()->stopCapture(); });
    connect(ui->actionChange_Video, &QAction::triggered, this, [this]{
        m_cameraCapture.data()->m_index_camera = !m_cameraCapture.data()->m_index_camera;
        m_rovDataParser.data()->m_control.data()->camsel = m_cameraCapture.data()->m_index_camera;
        m_cameraCapture.data()->setSource();
        m_cameraCapture.data()->startCapture();
        if (record_flag) {
            updateRecordStatus();
            m_cameraCapture.data()->stopRecord();
        }
    });
    connect(m_rovDataParser.data(), &RovDataParser::controlReady, this, [this] {
        if (m_cameraCapture.data()->m_index_camera != m_rovDataParser.data()->m_control.data()->camsel) {
            m_cameraCapture.data()->m_index_camera = m_rovDataParser.data()->m_control.data()->camsel;
            m_cameraCapture.data()->setSource();
            m_cameraCapture.data()->startCapture();
            if (record_flag) {
                updateRecordStatus();
                m_cameraCapture.data()->stopRecord();
            }
        }
    });
    connect(ui->actionDigiCam_report, &QAction::triggered, this,
            [this] { m_rovCameraCommunication->echo(); });
    connect(m_rovCameraCommunication.data(),
            &RovCameraCommunication::cameraSettingsReady, this,
            [this](QMap<QString, Setting> settings) {
                m_cameraSettings->recieveCameraSettings(settings);
                m_cameraSettings->show();
            });

    connect(m_cameraSettings.data(), &CameraSettings::updateCameraSettings,
            m_rovCameraCommunication.data(),
            [this](QMap<QString, Setting> settingsMap) {
                m_rovCameraCommunication->sendSettings(settingsMap);
            });
    connect(m_cameraSettings.data(), &CameraSettings::updateServo,
            m_rovCameraCommunication.data(),
            &RovCameraCommunication::updateServo);

    // Show setup dialogs
    connect(ui->actionDisplay_joystick_setup_dialog, &QAction::triggered, this,
            [this] { m_jsd->show(); });
    connect(ui->actionDisplay_rov_data_dialog, &QAction::triggered, this,
            [this] { m_rovDataParser->show(); });

    // Recalibrate IMU
    connect(ui->actionRecalibrate_IMU, &QAction::triggered,
            m_rovDataParser.data(), &RovDataParser::invalidateImuCalibration);

    // Show graphs
    connect(ui->actionShow_graphs, &QAction::triggered, this,
            [this] { m_datasplines->show(); });
    connect(ui->actionHide_graphs, &QAction::triggered, this,
            [this] { m_datasplines->hide(); });

    // JoystickHandler to:
    // JSD
    connect(m_joystickHandler.data(), SIGNAL(joystickUpdated(Joystick)),
            m_jsd.data(), SLOT(updateUi(Joystick)));
    connect(m_joystickHandler.data(), SIGNAL(joystickChanged(Joystick)),
            m_jsd.data(), SLOT(populateUi(Joystick)));
    // DataParser
    connect(m_joystickHandler.data(), SIGNAL(joystickUpdated(Joystick)),
            m_rovDataParser.data(), SLOT(prepareControl(Joystick)));

    // UI ASF
    connect(m_joystickHandler.data(), &JoystickHandler::joystickUpdated, this,
            [this](Joystick joy) {
                int8_t t_asf = ((joy.axis[6].axe / 100.0) + 1) / 2 * 100;
                if (t_asf > 100) t_asf = 100;
                if (t_asf < 0) t_asf = 0;
                ui->asfLabel->setText(QString::number(t_asf) + "% , " +  QString::number(joy.axis[0].runtimeASF));
            });

    // Settings update requests
    connect(m_jsd.data(), SIGNAL(settingsUpdated()), m_joystickHandler.data(),
            SLOT(updateSettings()));
    // DataParser:
    // Send AuxControl to ROV
    connect(m_rovDataParser.data(), SIGNAL(auxControlReady(QByteArray)),
            m_rovCommunication.data(), SLOT(write(QByteArray)));

    // Send RovControl to ROV
    connect(m_rovDataParser.data(), SIGNAL(controlReady(QByteArray)),
            m_rovCommunication.data(), SLOT(write(QByteArray)));

    // Process recieved RovTelemetry from ROV
    connect(m_rovCommunication.data(), SIGNAL(telemetryReady(QByteArray)),
            m_rovDataParser.data(), SLOT(processTelemetry(QByteArray)));

    // Update UI with telemetry data
    connect(m_rovDataParser.data(), SIGNAL(telemetryProcessed(RovTelemetry)),
            this, SLOT(updateTelemetry(RovTelemetry)));

    // Update DataSplines with telemetry data
    connect(m_rovDataParser.data(), SIGNAL(telemetryProcessed(RovTelemetry)),
            this, SLOT(updateDatasplines(RovTelemetry)));

    // Fix yaw
    connect(m_rovDataParser.data(), SIGNAL(telemetryProcessed(RovTelemetry)),
            this, SLOT(calculateYaw(RovTelemetry)));

    // Menu actions
    // Light on/off
    connect(ui->actionLight_on_off, SIGNAL(triggered(bool)),
            m_rovDataParser.data(), SLOT(toggleLight()));
    // Pump on/off
    connect(ui->actionPump_off_on, SIGNAL(triggered(bool)),
            m_rovDataParser.data(), SLOT(togglePump()));

    // Regulators:
    // Depth
    connect(ui->depthSpinBox, SIGNAL(valueChanged(double)),
            m_rovDataParser.data(), SLOT(setDepth(double)));
    connect(ui->depthRegulatorCB, SIGNAL(stateChanged(int)),
            m_rovDataParser.data(), SLOT(setDepthStatus(int)));
    connect(ui->actionDepthOff, &QAction::triggered, this, [this](bool) {
        ui->depthRegulatorCB->setCheckState(Qt::Unchecked);
    });
    connect(ui->actionDepthOn, &QAction::triggered, this,
            [this](bool) { ui->depthRegulatorCB->setCheckState(Qt::Checked); });
    connect(ui->actionDepthOnWC, &QAction::triggered, this, [this](bool) {
        ui->depthSpinBox->setValue(lastTele.depth);
        ui->depthRegulatorCB->setCheckState(Qt::Checked);
    });
    connect(m_joystickHandler.data(), &JoystickHandler::joystickUpdated, this,
            [this](Joystick joy) {
        if (joy.buttons.DepthRegButtonON) {
            ui->depthSpinBox->setValue(lastTele.depth);
            ui->depthRegulatorCB->setCheckState(Qt::Checked);
        }
        if (joy.buttons.DepthRegButtonOFF) {
            ui->depthRegulatorCB->setCheckState(Qt::Unchecked);
        }
    });
    // Yaw
    connect(ui->yawSpinBox, SIGNAL(valueChanged(double)),
            m_rovDataParser.data(), SLOT(setYaw(double)));
    connect(ui->yawRegulatorCB, SIGNAL(stateChanged(int)),
            m_rovDataParser.data(), SLOT(setYawStatus(int)));
    connect(ui->actionYawOff, &QAction::triggered, this,
            [this](bool) { ui->yawRegulatorCB->setCheckState(Qt::Unchecked); });
    connect(ui->actionYawOn, &QAction::triggered, this,
            [this](bool) { ui->yawRegulatorCB->setCheckState(Qt::Checked); });
    connect(ui->actionYawOnWC, &QAction::triggered, this, [this](bool) {
        ui->yawSpinBox->setValue(lastTele.yaw);
        ui->yawRegulatorCB->setCheckState(Qt::Checked);
    });
    // Roll
    connect(ui->rollSpinBox, SIGNAL(valueChanged(double)),
            m_rovDataParser.data(), SLOT(setRoll(double)));
    connect(ui->rollRegulatorCB, SIGNAL(stateChanged(int)),
            m_rovDataParser.data(), SLOT(setRollStatus(int)));
    connect(ui->actionRollOff, &QAction::triggered, this, [this](bool) {
        ui->rollRegulatorCB->setCheckState(Qt::Unchecked);
    });
    connect(ui->actionRollOn, &QAction::triggered, this,
            [this](bool) { ui->rollRegulatorCB->setCheckState(Qt::Checked); });
    connect(ui->actionRollOnWC, &QAction::triggered, this, [this](bool) {
        ui->rollSpinBox->setValue(lastTele.roll);
        ui->rollRegulatorCB->setCheckState(Qt::Checked);
    });
    // Pitch
    connect(ui->pitchSpinBox, SIGNAL(valueChanged(double)),
            m_rovDataParser.data(), SLOT(setPitch(double)));
    connect(ui->pitchRegulatorCB, SIGNAL(stateChanged(int)),
            m_rovDataParser.data(), SLOT(setPitchStatus(int)));
    connect(ui->actionPitchOff, &QAction::triggered, this, [this](bool) {
        ui->pitchRegulatorCB->setCheckState(Qt::Unchecked);
    });
    connect(ui->actionPitchOn, &QAction::triggered, this,
            [this](bool) { ui->pitchRegulatorCB->setCheckState(Qt::Checked); });
    connect(ui->actionPitchOnWC, &QAction::triggered, this, [this](bool) {
        ui->pitchSpinBox->setValue(lastTele.pitch);
        ui->pitchRegulatorCB->setCheckState(Qt::Checked);
    });
    //Servo digit change
    connect(m_rovDataParser.data(), SIGNAL(servoDigitReady(int)), m_rovCameraCommunication.data(), SLOT(updateServo(int)));
    // recording video
    connect(ui->recordButton, SIGNAL(clicked(bool)), this, SLOT(updateRecordStatus()));
    connect(this, SIGNAL(startRecord()), m_cameraCapture.data(), SLOT(startRecord()));
    connect(this, SIGNAL(stopRecord()), m_cameraCapture.data(), SLOT(stopRecord()));
}

void MainWindow::setupStatusbar() {
    m_rovStatusLabel->setText(" ROV status ");
    ui->statusbar->addPermanentWidget(m_rovStatusLabel.data());
    ui->statusbar->addPermanentWidget(m_rovStatusIndicator.data());
}
void MainWindow::updateRecordStatus() {
    if (!m_cameraCapture.data()->isRun()) return;

    record_flag = !record_flag;
    QColor colour;
    if (record_flag) colour = Qt::green;
    else colour = Qt::white;
    QPalette pal = ui->recordButton->palette();
    pal.setColor(QPalette::Button, QColor(colour));
    ui->recordButton->setAutoFillBackground(true);
    ui->recordButton->setPalette(pal);
    ui->recordButton->update();

    if (record_flag) emit startRecord();
    else emit stopRecord();
}

void MainWindow::calculateYaw(RovTelemetry tele) {
    yaweton += tele.yaw - yaweton;
    yaweton %= 360;
}
