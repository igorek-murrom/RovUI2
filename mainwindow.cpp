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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doUpdateCameraLabel(QImage img){
    ui->camLabel->setPixmap(QPixmap::fromImage(img));
    QSize targetQSize = QSize(ui->centralwidget->size().width(), ui->camLabel->heightForWidth(ui->centralwidget->size().width()));
    ui->camLabel->setMaximumSize(QSize(std::min(targetQSize.width(), ui->centralwidget->width()), std::min(targetQSize.height(), ui->centralwidget->height())));
    ui->camLabel->setMinimumSize(targetQSize);
    qDebug() << "X: " << ui->centralwidget->size().width() << " Y: " << ui->centralwidget->size().height();
    qDebug() << "lX: " << ui->camLabel->size().width() << " lY: " << ui->camLabel->size().height();
//    ui->camLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void MainWindow::doUpdateTelemetry(RovTelemetry telemetry){
    QString text;
    ui->teleVersionLabel->setText(       "Version:   " + QString::number(telemetry.version));
    ui->teleDepthLabel->setText(         "Depth:     " + QString::number(telemetry.depth));
    ui->telePitchLabel->setText(         "Pitch:     " + QString::number(telemetry.pitch));
    ui->teleYawLabel->setText(           "Yaw:       " + QString::number(telemetry.yaw));
    ui->teleRollLabel->setText(          "Roll:      " + QString::number(telemetry.roll));
    ui->teleTempLabel->setText(          "Twater:    " + QString::number(telemetry.temperature));
    ui->teleVoltageLabel->setText(       "Voltage:   " + QString::number(telemetry.voltmeter));
    ui->teleCurrentLabel->setText(       "Current:   " + QString::number(telemetry.ammeter));
    ui->teleCamSelLabel->setText(QString("CamSel:    ") + (telemetry.cameraIndex==0 ? "Front" : "Rear"));
}

void MainWindow::doUpdateThrustFactor(float factor){
    emit thrustFactorUpdated(factor);
}

void MainWindow::createConnections()
{
    connect(ui->actionBegin_camera_capture, &QAction::triggered, [this]{m_camera.data()->doStartCapture();});
    connect(ui->actionEnd_camera_capture, &QAction::triggered, [this]{m_camera.data()->doStopCapture();});

    connect(ui->actionDisplay_thruster_setup_dialog, &QAction::triggered, [this]{m_tsd->show();});
    connect(ui->actionDisplay_joystick_setup_dialog, &QAction::triggered, [this]{m_jsd->show();});

    connect(m_camera.data(), SIGNAL(imgProcessed(QImage)), this, SLOT(doUpdateCameraLabel(QImage)));

    connect(m_joystick.data(), SIGNAL(joystickUpdated(Joystick)), m_jsd.data(), SLOT(updateUi(Joystick)));
    connect(m_joystick.data(), SIGNAL(joystickChanged(Joystick)), m_jsd.data(), SLOT(populateUi(Joystick)));
    connect(m_jsd.data(), SIGNAL(settingsUpdated()), m_joystick.data(), SLOT(doUpdateSettings()));
    connect(m_joystick.data(), SIGNAL(joystickUpdated(Joystick)), m_dataparser.data(), SLOT(doPrepareDatagram(Joystick)));

    connect(m_dataparser.data(), SIGNAL(controlReady(QByteArray)), m_communication.data(), SLOT(doSendControl(QByteArray)));
    connect(m_communication.data(), SIGNAL(telemetryReady(QByteArray)), m_dataparser.data(), SLOT(doProcessTelemetry(QByteArray)));
    connect(m_dataparser.data(), SIGNAL(telemetryReady(RovTelemetry)), this, SLOT(doUpdateTelemetry(RovTelemetry)));
}
