#ifndef JOYSTICKHANDLER_H
#define JOYSTICKHANDLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <SDL.h>
#include "joystick.h"

class JoystickHandler : public QObject
{
    Q_OBJECT
public:
    explicit JoystickHandler(QObject *parent = nullptr);

signals:
    void joystickUpdated(Joystick);
    void joystickChanged(Joystick);
public slots:
    void doUpdateSettings();
    void doUpdateJoystick();

private slots:
    void timerEvent(QTimerEvent *event);

private:

    inline float map(float x, float in_min, float in_max, float out_min, float out_max){
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    QScopedPointer<Joystick> m_joystick;
    QScopedPointer<QSettings> m_settings;
    QString m_axesNames[6] = { "AxisX", "AxisY", "AxisZ",
                               "AxisW", "AxisD", "AxisR" };
    QString m_buttonsNames[16] = {
                                 "ManipOpen", "ManipClose", "CamSel", "TSF-F",
                                 "TSF-M", "TSF-S", "Button7", "Button8",
                                 "Button9", "Button10", "Button11", "Button12",
                                 "Button13", "Button14", "Button15", "Button16"};
    QString m_hatsNames[4] = { "CamServo", "Hat2", "Hat3", "Hat4"};
    bool notifiedNoJoysticks = false;
    bool notifiedJoystickDisconnected = false;
    bool joystickConnected = false;
};

#endif // JOYSTICKHANDLER_H
