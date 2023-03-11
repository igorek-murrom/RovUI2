#include "mainwindow.h"
#include "ui_mainwindow2.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_camera(new RovCameraCapture(this))
    , m_tsd(new ThrusterSetupDialog(this))
    , m_joystick(new JoystickHandler(this))
    , m_jsd(new JoystickSetupDialog(this))
    , m_communication(new RovCommunication(this))
    , m_dataparser(new RovDataParser(this))
{
    ui->setupUi(this);
    setWindowTitle("RovUI2 v0.95");
    createConnections();
    doUpdateTelemetry(RovTelemetry());
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::doResizeCameraLabel);
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doResizeCameraLabel(){
    QSize targetQSize = QSize(ui->centralwidget->size().width(), ui->camLabel->heightForWidth(ui->centralwidget->size().width()));
    ui->camLabel->setMaximumSize(QSize(std::min(targetQSize.width(), ui->centralwidget->width()), std::min(targetQSize.height(), ui->centralwidget->height())));
    ui->camLabel->setMinimumSize(targetQSize);
}

void MainWindow::doUpdateCameraLabel(QImage img){
    ui->camLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::doUpdateTelemetry(RovTelemetry telemetry){
    QString text;
    ui->teleVersionLabel->setText(       "Version:   " + QString::number(telemetry.version));
    ui->teleDepthLabel->setText(         "Depth:     " + QString::number(telemetry.depth) + " m");
    ui->telePitchLabel->setText(         "Pitch:     " + QString::number(telemetry.pitch) + " deg");
    ui->teleYawLabel->setText(           "Yaw:       " + QString::number(telemetry.yaw) + " deg");
    ui->teleRollLabel->setText(          "Roll:      " + QString::number(telemetry.roll) + " deg");
    ui->teleTempLabel->setText(          "Twater:    " + QString::number(telemetry.temperature) + " C");
    ui->teleVoltageLabel->setText(       "Voltage:   " + QString::number(std::round(telemetry.voltmeter * 100)/100) + " V");
    ui->teleCurrentLabel->setText(       "Current:   " + QString::number(std::round(telemetry.ammeter * 100000)/100) + " mA");
    ui->teleCamSelLabel->setText(QString("CamSel:    ") + (telemetry.cameraIndex==0 ? "Front" : "Rear"));
}

void MainWindow::doUpdateThrustFactor(float factor){
    emit thrustFactorUpdated(factor);
}

void MainWindow::createConnections()
{
    connect(ui->actionBegin_camera_capture, &QAction::triggered, [this]{m_camera.data()->doStartCapture();});
    connect(ui->actionEnd_camera_capture, &QAction::triggered, [this]{m_camera.data()->doStopCapture();});

    connect(m_camera.data(), SIGNAL(imgProcessed(QImage)), this, SLOT(doUpdateCameraLabel(QImage)));

    connect(ui->actionDisplay_thruster_setup_dialog, &QAction::triggered, [this]{m_tsd->show();});
    connect(ui->actionDisplay_joystick_setup_dialog, &QAction::triggered, [this]{m_jsd->show();});


    connect(m_joystick.data(), SIGNAL(joystickUpdated(Joystick)), m_jsd.data(), SLOT(updateUi(Joystick)));
    connect(m_joystick.data(), SIGNAL(joystickChanged(Joystick)), m_jsd.data(), SLOT(populateUi(Joystick)));
    connect(m_joystick.data(), SIGNAL(joystickUpdated(Joystick)), m_dataparser.data(), SLOT(doPrepareDatagram(Joystick)));
    connect(m_jsd.data(), SIGNAL(settingsUpdated()), m_joystick.data(), SLOT(doUpdateSettings()));

    connect(m_dataparser.data(), SIGNAL(controlReady(QByteArray)), m_communication.data(), SLOT(doSendControl(QByteArray)));
    connect(m_communication.data(), SIGNAL(telemetryReady(QByteArray)), m_dataparser.data(), SLOT(doProcessTelemetry(QByteArray)));
    connect(m_dataparser.data(), SIGNAL(telemetryProcessed(RovTelemetry)), this, SLOT(doUpdateTelemetry(RovTelemetry)));

    connect(m_tsd.data(), SIGNAL(overrideStatusChanged(bool)), m_dataparser.data(), SLOT(doEnableThrustersOverride(bool)));
    connect(m_tsd.data(), SIGNAL(thrustersOverridden(QList<qint8>)), m_dataparser.data(), SLOT(doSetThrustersOverride(QList<qint8>)));
    connect(m_tsd.data(), SIGNAL(overrideStatusChanged(bool)), m_dataparser.data(), SLOT(doEnableThrustersOverride(bool)));
}
