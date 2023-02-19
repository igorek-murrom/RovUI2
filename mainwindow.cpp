#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_camera(new RovCameraCapture(this))
    , m_tdd(new ThrusterSetupDialog(this))
    , m_joystick(new JoystickHandler(this))
    , m_jsd(new JoystickSetupDialog(this))
    , m_communication(new RovCommunication(this))
    , m_dataparser(new RovDataParser(this))
{
    ui->setupUi(this);
    setCentralWidget(ui->camLabel);
    createActions();
    createConnections();
    createMenuBar();
    doUpdateTelemetry(RovTelemetry());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doUpdateCameraLabel(QImage img){
    ui->camLabel->setPixmap(QPixmap::fromImage(img));
    ui->camLabel->setScaledContents(true);
    ui->camLabel->setMaximumSize(QSize(this->size().width()-250, (this->size().width()-250)*9/16));
    ui->camLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void MainWindow::doUpdateTelemetry(RovTelemetry telemetry){
    QString text;
    ui->telemetryLabel->setText(     "Telemetry:");
    ui->tVersionLabel->setText(      "Version:   " + QString::number(telemetry.version));
    ui->tDepthLabel->setText(        "Depth:     " + QString::number(telemetry.depth));
    ui->tPitchLabel->setText(        "Pitch:     " + QString::number(telemetry.pitch));
    ui->tYawLabel->setText(          "Yaw:       " + QString::number(telemetry.yaw));
    ui->tRollLabel->setText(         "Roll:      " + QString::number(telemetry.roll));
    ui->tTemperaturLabel->setText(   "Temp:      " + QString::number(telemetry.temperature));
    ui->tCamSelLabel->setText(QString("CamSel:    ") + (telemetry.cameraIndex==0 ? "Front" : "Rear"));
    ui->telemetryDock->setMinimumHeight(50);
}

void MainWindow::doUpdateThrustFactor(float factor){
    emit thrustFactorUpdated(factor);
}

void MainWindow::createActions(){
    m_cameraBeginCapture.reset(new QAction("Begin camera capture"));
    m_cameraEndCapture.reset(new QAction("End camera capture"));
    m_thrustersOpenDialog.reset(new QAction("Show thruster setup dialog"));
    m_joystickOpenDialog.reset(new QAction("Show joystick setup dialog"));
}

void MainWindow::createConnections()
{
    connect(m_cameraBeginCapture.data(), &QAction::triggered, [this]{m_camera.data()->doStartCapture();});
    connect(m_cameraEndCapture.data(), &QAction::triggered, [this]{m_camera.data()->doStopCapture();});

    connect(m_thrustersOpenDialog.data(), &QAction::triggered, [this]{m_tdd->show();});
    connect(m_joystickOpenDialog.data(), &QAction::triggered, [this]{m_jsd->show();});

    connect(m_camera.data(), SIGNAL(imgProcessed(QImage)), this, SLOT(doUpdateCameraLabel(QImage)));

    connect(m_joystick.data(), SIGNAL(joystickUpdated(Joystick)), m_jsd.data(), SLOT(updateUi(Joystick)));
    connect(m_joystick.data(), SIGNAL(joystickChanged(Joystick)), m_jsd.data(), SLOT(populateUi(Joystick)));
    connect(m_jsd.data(), SIGNAL(settingsUpdated()), m_joystick.data(), SLOT(doUpdateSettings()));
    connect(m_joystick.data(), SIGNAL(joystickUpdated(Joystick)), m_dataparser.data(), SLOT(doPrepareDatagram(Joystick)));

    connect(m_dataparser.data(), SIGNAL(controlReady(QByteArray)), m_communication.data(), SLOT(doSendControl(QByteArray)));
    connect(m_communication.data(), SIGNAL(telemetryReady(QByteArray)), m_dataparser.data(), SLOT(doProcessTelemetry(QByteArray)));
    connect(m_dataparser.data(), SIGNAL(telemetryReady(RovTelemetry)), this, SLOT(doUpdateTelemetry(RovTelemetry)));
}

void MainWindow::createMenuBar(){
    QMenu *camera = ui->menubar->addMenu("Camera");
    camera->addAction(m_cameraBeginCapture.data());
    camera->addAction(m_cameraEndCapture.data());

    QMenu *thrusters = ui->menubar->addMenu("Thrusters");
    thrusters->addAction(m_thrustersOpenDialog.data());

    QMenu *joystick = ui->menubar->addMenu("Joystick");
    joystick->addAction(m_joystickOpenDialog.data());

}
