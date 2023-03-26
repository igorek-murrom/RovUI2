#ifndef JOYSTICKHANDLER_H
#define JOYSTICKHANDLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <SDL.h>
#include <SDL_joystick.h>
#include "joystick.h"
#include "helpers.h"
#include "qmutex.h"

class JoystickHandler : public QObject
{
    Q_OBJECT
public:
    explicit JoystickHandler(QObject *parent = nullptr);

signals:
    void joystickUpdated(Joystick);
    void joystickChanged(Joystick);
public slots:
    void updateSettings();
    void updateJoystick();
    void updateASFs(int[6]);
private slots:
    void timerEvent(QTimerEvent *event);

private:

    void eventLoop();
    void parseAxisEvent(SDL_Event event);
    void parseButtonEvent(SDL_Event event);
    void parseHatEvent(SDL_Event event);

    inline float map(float x, float in_min, float in_max, float out_min, float out_max){
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    QMutex m_joystickMutex;
    QScopedPointer<Joystick> m_joystick;
    SDL_Joystick *m_sdlJoystick;

    QScopedPointer<QSettings> m_settings;

    bool notifiedNoJoysticks = false;
    bool notifiedJoystickDisconnected = false;
    bool joystickConnected = false;
};

#endif // JOYSTICKHANDLER_H
