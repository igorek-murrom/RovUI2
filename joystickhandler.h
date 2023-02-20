#ifndef JOYSTICKHANDLER_H
#define JOYSTICKHANDLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <SDL.h>
#include "joystick.h"
#include "bithelpers.h"

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
    void doUpdateASFs(int[6]);
private slots:
    void timerEvent(QTimerEvent *event);

private:

    inline float map(float x, float in_min, float in_max, float out_min, float out_max){
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    QScopedPointer<Joystick> m_joystick;
    QScopedPointer<QSettings> m_settings;
    bool notifiedNoJoysticks = false;
    bool notifiedJoystickDisconnected = false;
    bool joystickConnected = false;
};

#endif // JOYSTICKHANDLER_H
