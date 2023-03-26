#include "mainwindow.h"
#include "ui_mainwindow2.h"
#include "unistd.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_camera(new RovCameraCapture(this))
    , m_tsd(new ThrusterSetupDialog(this))
    , m_joystick(new JoystickHandler(this))
    , m_jsd(new JoystickSetupDialog(this))
    , m_communication(new RovCommunication(this))
    , m_dataparser(new RovDataParser(this))
    , m_datasplines(new RovDataSplines(this))

    , m_rovStatusLabel(new QLabel())
    , m_rovStatusIndicator(new LEDIndicator())
{
    ui->setupUi(this);
    setWindowTitle("RovUI2 v0.95");

    setupStatusbar();
    updateStatusbarText("Initialization...");

    createConnections();
    updateTelemetry(RovTelemetry());

    QTimer *cameraLabelTimer = new QTimer(this);
    connect(cameraLabelTimer, &QTimer::timeout, this, &MainWindow::resizeCameraLabel);
    cameraLabelTimer->start();

    QTimer *statusbarIndicatorsTimer = new QTimer(this);
    connect(statusbarIndicatorsTimer, &QTimer::timeout, this, &MainWindow::updateStatusbarIndicators);
    cameraLabelTimer->start(32);

    updateStatusbarText("RovUI2 initialization complete ^_^");
    updateStatusbarProgress(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeCameraLabel(){
    QSize targetQSize = QSize(ui->centralwidget->size().width(), ui->camLabel->heightForWidth(ui->centralwidget->size().width()));
    ui->camLabel->setMaximumSize(QSize(std::min(targetQSize.width(), ui->centralwidget->width()), std::min(targetQSize.height(), ui->centralwidget->height())));
    ui->camLabel->setMinimumSize(targetQSize);
}

void MainWindow::updateCameraLabel(QImage img){
    ui->camLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::updateDatasplines(RovTelemetry tele){
    m_datasplines.data()->addVoltageSample(QPointF(m_vSamples++, tele.voltmeter));
    m_datasplines.data()->addCurrentSample(QPointF(m_aSamples++, tele.current));
}

void MainWindow::updateStatusbarText(QString status){
    ui->statusbar->showMessage(status, 0);
}

void MainWindow::updateStatusbarProgress(int progress){
    QProgressBar *pb = new QProgressBar(ui->statusbar);
    pb->setValue(progress);
    ui->statusbar->addWidget(pb);
}

void MainWindow::updateStatusbarIndicators(){

}

void MainWindow::setDesiredDepth(float val){
    ui->depthSpinBox->setValue(val);
}
void MainWindow::setDesiredYaw(float val){
    ui->yawSpinBox->setValue(val);
}
void MainWindow::setDesiredRoll(float val){
    ui->rollSpinBox->setValue(val);
}
void MainWindow::setDesiredPitch(float val){
    ui->pitchSpinBox->setValue(val);
}

void MainWindow::overrideDepth(bool readonly){
    ui->depthSpinBox->setReadOnly(readonly);
    ui->depthRegulatorCB->setCheckable(readonly);
}
void MainWindow::overrideYaw(bool readonly){
    ui->yawSpinBox->setReadOnly(readonly);
    ui->yawRegulatorCB->setCheckable(readonly);
}
void MainWindow::overrideRoll(bool readonly){
    ui->rollSpinBox->setReadOnly(readonly);
    ui->rollRegulatorCB->setCheckable(readonly);
}
void MainWindow::overridePitch(bool readonly){
    ui->pitchSpinBox->setReadOnly(readonly);
    ui->pitchRegulatorCB->setCheckable(readonly);
}

void MainWindow::updateTelemetry(RovTelemetry telemetry){
    ui->teleVersionLabel->setText("Version:   " + QString::number(telemetry.version));
    ui->teleDepthLabel->setText(  "Depth:     " + QString::number(telemetry.depth) + " m");
    ui->telePitchLabel->setText(  "Pitch:     " + QString::number(telemetry.pitch) + " deg");
    ui->teleYawLabel->setText(    "Yaw:       " + QString::number(telemetry.yaw) + " deg");
    ui->teleRollLabel->setText(   "Roll:      " + QString::number(telemetry.roll) + " deg");
    ui->teleTempLabel->setText(   "Twater:    " + QString::number(telemetry.temperature) + " C");
    ui->teleVoltageLabel->setText("Voltage:   " + QString::number(std::round(telemetry.voltmeter * 100)/100) + " V");
    ui->teleCurrentLabel->setText("Current:   " + QString::number(std::round(telemetry.current * 100000)/100) + " mA");
    ui->teleCamSelLabel->setText( "CamSel:    " + QString((telemetry.cameraIndex==0 ? "Front" : "Rear")));
}

void MainWindow::updateASF(float factor){
    emit asfUpdated(factor);
}

void MainWindow::createConnections()
{
    connect(ui->actionBegin_camera_capture, &QAction::triggered, this, [this]{m_camera.data()->startCapture(0);});
    connect(ui->actionEnd_camera_capture, &QAction::triggered, this, [this]{m_camera.data()->stopCapture();});

    connect(m_camera.data(), SIGNAL(imgProcessed(QImage)), this, SLOT(updateCameraLabel(QImage)));

    connect(ui->actionDisplay_thruster_setup_dialog, &QAction::triggered, this, [this]{m_tsd->show();});
    connect(ui->actionDisplay_joystick_setup_dialog, &QAction::triggered, this, [this]{m_jsd->show();});

    connect(ui->actionShow_graphs, &QAction::triggered, this, [this]{ m_datasplines->show();});
    connect(ui->actionHide_graphs, &QAction::triggered, this, [this]{ m_datasplines->hide();});

    connect(m_joystick.data(), SIGNAL(joystickUpdated(Joystick)), m_jsd.data(), SLOT(updateUi(Joystick)));
    connect(m_joystick.data(), SIGNAL(joystickChanged(Joystick)), m_jsd.data(), SLOT(populateUi(Joystick)));
    connect(m_joystick.data(), SIGNAL(joystickUpdated(Joystick)), m_dataparser.data(), SLOT(prepareControl(Joystick)));
    connect(m_jsd.data(), SIGNAL(settingsUpdated()), m_joystick.data(), SLOT(updateSettings()));

    connect(ui->depthSpinBox, SIGNAL(valueChanged(double)), m_dataparser.data(), SLOT(setDepth(double)));
    connect(ui->yawSpinBox, SIGNAL(valueChanged(double)), m_dataparser.data(), SLOT(setYaw(double)));
    connect(ui->rollSpinBox, SIGNAL(valueChanged(double)), m_dataparser.data(), SLOT(setRoll(double)));
    connect(ui->pitchSpinBox, SIGNAL(valueChanged(double)), m_dataparser.data(), SLOT(setPitch(double)));

    connect(m_dataparser.data(), SIGNAL(auxControlReady(QByteArray)), m_communication.data(), SLOT(write(QByteArray)));

    connect(m_dataparser.data(), SIGNAL(controlReady(QByteArray)), m_communication.data(), SLOT(write(QByteArray)));
    connect(m_communication.data(), SIGNAL(telemetryReady(QByteArray)), m_dataparser.data(), SLOT(processTelemetry(QByteArray)));
    connect(m_dataparser.data(), SIGNAL(telemetryProcessed(RovTelemetry)), this, SLOT(updateTelemetry(RovTelemetry)));

    connect(m_tsd.data(), SIGNAL(overrideStatusChanged(bool)), m_dataparser.data(), SLOT(enableThrustersOverride(bool)));
    connect(m_tsd.data(), SIGNAL(thrustersOverridden(QList<qint8>)), m_dataparser.data(), SLOT(setThrustersOverride(QList<qint8>)));
    connect(m_tsd.data(), SIGNAL(overrideStatusChanged(bool)), m_dataparser.data(), SLOT(enableThrustersOverride(bool)));

    connect(m_dataparser.data(), SIGNAL(telemetryProcessed(RovTelemetry)), this, SLOT(updateDatasplines(RovTelemetry)));
}

void MainWindow::setupStatusbar(){
    m_rovStatusLabel->setText(" ROV status ");
    ui->statusbar->addPermanentWidget(m_rovStatusLabel.data());
    ui->statusbar->addPermanentWidget(m_rovStatusIndicator.data());
}
