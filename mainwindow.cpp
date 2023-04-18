#include "mainwindow.h"
#include "ui_mainwindow2.h"
#include "unistd.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      m_cameraCapture(new RovCameraCapture(this)),
      m_tsd(new ThrusterSetupDialog(this)),
      m_joystickHandler(new JoystickHandler(this)),
      m_jsd(new JoystickSetupDialog(this)),
      m_rovCommunication(new RovCommunication(this)),
      m_rovDataParser(new RovDataParser(this)),
      m_datasplines(new RovDataSplines(this))

      ,
      m_rovStatusLabel(new QLabel()), m_rovStatusIndicator(new LEDIndicator()) {
    ui->setupUi(this);
    setWindowTitle("RovUI2 v0.95");

    setupStatusbar();
    updateStatusbarText("Initialization...");

    createConnections();
    updateTelemetry(RovTelemetry());

    QTimer *cameraLabelTimer = new QTimer(this);
    connect(cameraLabelTimer, &QTimer::timeout, this,
            &MainWindow::resizeCameraLabel);
    cameraLabelTimer->start(16);

    QTimer *statusbarIndicatorsTimer = new QTimer(this);
    connect(statusbarIndicatorsTimer, &QTimer::timeout, this,
            &MainWindow::updateStatusbarIndicators);

    updateStatusbarText("RovUI2 initialization complete ^_^");
    updateStatusbarProgress(100);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeCameraLabel() {
    int targetWidth = ui->camLabel->heightForWidth(ui->camLabel->width());
    int targetHeight = ui->camLabel->widthForHeight(ui->camLabel->height());
    if (targetHeight >
        ui->centralwidget->height() -
            12) { // if wanted height is larger than the available height
        targetHeight =
            ui->camLabel->heightForWidth(ui->centralwidget->width() - 12);
        targetWidth = ui->camLabel->widthForHeight(targetHeight);
        ui->camLabel->setFixedSize(QSize(targetWidth, targetHeight));
    }
    if (targetWidth >
        ui->centralwidget->width() -
            12) { // if wanted width is larger than the available width
        targetWidth =
            ui->camLabel->widthForHeight(ui->centralwidget->height() - 12);
        targetHeight = ui->camLabel->heightForWidth(targetWidth);
        ui->camLabel->setFixedSize(QSize(targetWidth, targetHeight));
    }
    ui->camLabel->setFixedSize(QSize(targetWidth, targetHeight));
}

void MainWindow::updateCameraLabel(QImage img) {
    ui->camLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::updateDatasplines(RovTelemetry tele) {
    m_datasplines.data()->addVoltageSample(QPointF(m_vSamples++, tele.voltage));
    m_datasplines.data()->addCurrentSample(QPointF(m_aSamples++, tele.current));
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
    ui->depthRegulatorCB->setCheckable(readonly);
}
void MainWindow::overrideYaw(bool readonly) {
    ui->yawSpinBox->setReadOnly(readonly);
    ui->yawRegulatorCB->setCheckable(readonly);
}
void MainWindow::overrideRoll(bool readonly) {
    ui->rollSpinBox->setReadOnly(readonly);
    ui->rollRegulatorCB->setCheckable(readonly);
}
void MainWindow::overridePitch(bool readonly) {
    ui->pitchSpinBox->setReadOnly(readonly);
    ui->pitchRegulatorCB->setCheckable(readonly);
}

void MainWindow::updateTelemetry(RovTelemetry telemetry) {
    ui->teleVersionLabel->setText(QString::number(telemetry.version));
    ui->teleDepthLabel->setText(
        (telemetry.depth < 3.4E+38
             ? QString::number(telemetry.depth) + QString(" m")
             : "[DIS]"));
    ui->telePitchLabel->setText(QString::number(telemetry.pitch) + " deg");
    ui->teleYawLabel->setText(QString::number(telemetry.yaw) + " deg");
    ui->teleRollLabel->setText(QString::number(telemetry.roll) + " deg");
    ui->teleTemperatureLabel->setText(QString::number(telemetry.temperature) +
                                      " C");
    ui->teleVoltageLabel->setText(
        QString::number(std::round(telemetry.voltage * 100) / 100) + " V");
    ui->teleCurrentLabel->setText(
        QString::number(std::round(telemetry.current * 100000) / 100) + " mA");
    ui->teleCamSelLabel->setText(
        QString((telemetry.cameraIndex == 0 ? "Front" : "Rear")));
}

void MainWindow::updateASF(float factor) { emit asfUpdated(factor); }

void MainWindow::createConnections() {
    connect(ui->actionBegin_camera_capture, &QAction::triggered, this,
            [this] { m_cameraCapture.data()->startCapture(2); });
    connect(ui->actionEnd_camera_capture, &QAction::triggered, this,
            [this] { m_cameraCapture.data()->stopCapture(); });

    connect(m_cameraCapture.data(), SIGNAL(imgProcessed(QImage)), this,
            SLOT(updateCameraLabel(QImage)));

    connect(ui->actionDisplay_thruster_setup_dialog, &QAction::triggered, this,
            [this] { m_tsd->show(); });
    connect(ui->actionDisplay_joystick_setup_dialog, &QAction::triggered, this,
            [this] { m_jsd->show(); });

    connect(ui->actionShow_graphs, &QAction::triggered, this,
            [this] { m_datasplines->show(); });
    connect(ui->actionHide_graphs, &QAction::triggered, this,
            [this] { m_datasplines->hide(); });

    connect(m_joystickHandler.data(), SIGNAL(joystickUpdated(Joystick)),
            m_jsd.data(), SLOT(updateUi(Joystick)));
    connect(m_joystickHandler.data(), SIGNAL(joystickChanged(Joystick)),
            m_jsd.data(), SLOT(populateUi(Joystick)));
    connect(m_joystickHandler.data(), SIGNAL(joystickUpdated(Joystick)),
            m_rovDataParser.data(), SLOT(prepareControl(Joystick)));
    connect(m_jsd.data(), SIGNAL(settingsUpdated()), m_joystickHandler.data(),
            SLOT(updateSettings()));

    connect(ui->depthSpinBox, SIGNAL(valueChanged(double)),
            m_rovDataParser.data(), SLOT(setDepth(double)));
    connect(ui->depthRegulatorCB, SIGNAL(stateChanged(int)),
            m_rovDataParser.data(), SLOT(setDepthStatus(int)));

    connect(ui->yawSpinBox, SIGNAL(valueChanged(double)),
            m_rovDataParser.data(), SLOT(setYaw(double)));
    connect(ui->yawRegulatorCB, SIGNAL(stateChanged(int)),
            m_rovDataParser.data(), SLOT(setYawStatus(int)));

    connect(ui->rollSpinBox, SIGNAL(valueChanged(double)),
            m_rovDataParser.data(), SLOT(setRoll(double)));
    connect(ui->rollRegulatorCB, SIGNAL(stateChanged(int)),
            m_rovDataParser.data(), SLOT(setRollStatus(int)));

    connect(ui->pitchSpinBox, SIGNAL(valueChanged(double)),
            m_rovDataParser.data(), SLOT(setPitch(double)));
    connect(ui->pitchRegulatorCB, SIGNAL(stateChanged(int)),
            m_rovDataParser.data(), SLOT(setPitchStatus(int)));

    connect(m_rovDataParser.data(), SIGNAL(auxControlReady(QByteArray)),
            m_rovCommunication.data(), SLOT(write(QByteArray)));

    connect(m_rovDataParser.data(), SIGNAL(controlReady(QByteArray)),
            m_rovCommunication.data(), SLOT(write(QByteArray)));
    connect(m_rovCommunication.data(), SIGNAL(telemetryReady(QByteArray)),
            m_rovDataParser.data(), SLOT(processTelemetry(QByteArray)));
    connect(m_rovDataParser.data(), SIGNAL(telemetryProcessed(RovTelemetry)),
            this, SLOT(updateTelemetry(RovTelemetry)));

    connect(m_tsd.data(), SIGNAL(overrideStatusChanged(bool)),
            m_rovDataParser.data(), SLOT(enableThrustersOverride(bool)));
    connect(m_tsd.data(), SIGNAL(thrustersOverridden(QList<qint8>)),
            m_rovDataParser.data(), SLOT(setThrustersOverride(QList<qint8>)));
    connect(m_tsd.data(), SIGNAL(overrideStatusChanged(bool)),
            m_rovDataParser.data(), SLOT(enableThrustersOverride(bool)));

    connect(m_rovDataParser.data(), SIGNAL(telemetryProcessed(RovTelemetry)),
            this, SLOT(updateDatasplines(RovTelemetry)));
}

void MainWindow::setupStatusbar() {
    m_rovStatusLabel->setText(" ROV status ");
    ui->statusbar->addPermanentWidget(m_rovStatusLabel.data());
    ui->statusbar->addPermanentWidget(m_rovStatusIndicator.data());
}
