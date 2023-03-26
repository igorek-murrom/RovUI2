#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <chrono>
#include "math.h"
#include "joystickhandler.h"
#include "joysticksetupdialog.h"
#include "rovcameracapture.h"
#include "rovdatasplines.h"
#include "thrustersetupdialog.h"
#include "rovcommunication.h"
#include "rovdataparser.h"
#include "rovdatatypes.h"
#include "ledindicator.h"

static QString settings_path = QString(getenv("HOME")) + QString("/.config/RovUI2.ini");

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateCameraLabel(QImage);
    void resizeCameraLabel();
    void updateStatusbarIndicators();
    void updateTelemetry(RovTelemetry);
    void updateDatasplines(RovTelemetry);
    void updateStatusbarText(QString);
    void updateStatusbarProgress(int);
//    void updateRegulators(QBitArray regulators); //TODO: implement
    void updateASF(float thrustFactor);
    void setDesiredDepth(float val);
    void setDesiredYaw(float val);
    void setDesiredPitch(float val);
    void setDesiredRoll(float val);
    void overrideDepth(bool ro);
    void overrideYaw(bool ro);
    void overridePitch(bool ro);
    void overrideRoll(bool ro);

signals:
    void asfUpdated(float thrustFactor); // TODO: implement

private:
    void createConnections();
    void setupStatusbar();

    quint64 m_vSamples = 0;
    quint64 m_aSamples = 0;

    std::chrono::time_point<std::chrono::system_clock> m_lastTelemetryRecieveTime;

    Ui::MainWindow *ui;
    QScopedPointer<RovCameraCapture> m_camera;
    QScopedPointer<ThrusterSetupDialog> m_tsd;
    QScopedPointer<JoystickHandler> m_joystick;
    QScopedPointer<JoystickSetupDialog> m_jsd;
    QScopedPointer<RovCommunication> m_communication;
    QScopedPointer<RovDataParser> m_dataparser;
    QScopedPointer<RovDataSplines> m_datasplines;

    QScopedPointer<QLabel> m_rovStatusLabel;
    QScopedPointer<LEDIndicator> m_rovStatusIndicator;
};
#endif // MAINWINDOW_H
