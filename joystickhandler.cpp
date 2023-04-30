#include "joystickhandler.h"

JoystickHandler::JoystickHandler(QObject *parent)
    : QObject{parent}, m_joystick(new Joystick),
      m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope,
                               "CfRD", "RovUI2", this)) {
    SDL_Init(SDL_INIT_JOYSTICK);
    SDL_JoystickUpdate();
    if (SDL_NumJoysticks() > 0) {
        m_sdlJoystick     = SDL_JoystickOpen(0);
        joystickConnected = true;
        updateJoystick();
        updateSettings();
        QTimer::singleShot(200, this, [this]() {
            emit joystickChanged(Joystick(m_joystick.data()));
        });
    }
    startTimer(16);
}

void JoystickHandler::updateSettings() {
    m_settings.data()->sync();
    for (int i = 0; i < 6; i++) // init axes from settings
        m_joystick.data()->axes_id[i] =
            m_settings
                ->value(QString(SDL_JoystickName(m_sdlJoystick)) +
                        QString("/joystickAxes/") +
                        JoystickHelpers::axesNames[i])
                .toInt();
    for (int i = 0; i < 16; i++) // same for buttons
        m_joystick.data()->buttons_id[i] =
            m_settings
                ->value(QString(SDL_JoystickName(m_sdlJoystick)) +
                        QString("/joystickButtons/") +
                        JoystickHelpers::buttonsNames[i])
                .toInt();
    for (int i = 0; i < 4; i++) { // same for hats
        m_joystick.data()->hats_id[i] =
            m_settings
                ->value(QString(SDL_JoystickName(m_sdlJoystick)) +
                        QString("/joystickHats/") +
                        JoystickHelpers::hatsNames[i])
                .toInt();
        m_joystick.data()->hats_hor[i] =
            m_settings
                ->value(QString(SDL_JoystickName(m_sdlJoystick)) +
                        QString("/joystickHatsHor/") +
                        JoystickHelpers::hatsNames[i])
                .toBool();
    }
}

void JoystickHandler::updateJoystick() {
    m_joystick->numAxes      = SDL_JoystickNumAxes(m_sdlJoystick);
    m_joystick->numHats      = SDL_JoystickNumHats(m_sdlJoystick);
    m_joystick->numButtons   = SDL_JoystickNumButtons(m_sdlJoystick);
    m_joystick->joystickName = QString(SDL_JoystickName(m_sdlJoystick));
    emit joystickChanged(Joystick(m_joystick.data()));
}

void JoystickHandler::updateASFs(int asf[6]) {
    for (int i = 0; i < 8; ++i) {
        m_joystick->runtimeASF[i] = asf[i];
    }
}

void JoystickHandler::timerEvent(QTimerEvent *) {
    m_settings.data()->sync();
    SDL_JoystickUpdate();
    if (SDL_NumJoysticks() > 0) {
        if (m_sdlJoystick != nullptr && joystickConnected == true) {
            if (SDL_JoystickGetAttached(
                    m_sdlJoystick)) { // joystick should exist and be opened
                for (size_t i = 0; i < helpers::size(m_joystick.data()->axes);
                     i++) { // joystick input smoothing 3x
                    if (m_joystick.data()->axes_id[i] >= 0) {
                        float currentAxisVal = SDL_JoystickGetAxis(
                            m_sdlJoystick, m_joystick.data()->axes_id[i]);
                        float lastAxisVal    = m_joystick.data()->axes[i];
                        float secLastAxisVal = m_joystick.data()->axes_last[i];

                        m_joystick.data()->axes[i] =
                            (map(currentAxisVal, -32768, 32767, -101, 101) +
                             lastAxisVal + secLastAxisVal) /
                            3;
                        m_joystick.data()->axes_last[i] = lastAxisVal;
                    } else {
                        m_joystick.data()->axes[i]      = 0;
                        m_joystick.data()->axes_last[i] = 0;
                    }
                }

                for (size_t i = 0; i < sizeof(m_joystick.data()->buttons) * 8;
                     i++) { // write buttons into the variable according to
                            // buttonsNames
                    if (SDL_JoystickGetButton(m_sdlJoystick,
                                              m_joystick.data()->buttons_id[i]))
                        BIT_SET(m_joystick.data()->buttons.rawData, i);
                    else
                        BIT_CLEAR(m_joystick.data()->buttons.rawData, i);
                }

                for (size_t i = 0; i < helpers::size(m_joystick.data()->hats);
                     i++) { // write hats

                    int hat = SDL_JoystickGetHat(m_sdlJoystick,
                                                 m_joystick.data()->hats_id[i]);
                    if (m_joystick.data()
                            ->hats_hor[i]) { // left -> -1, centered -> 0,
                                             // right -> 1, else -> 0
                        m_joystick.data()->hats[i] = (hat & SDL_HAT_LEFT) ? -1
                                                     : (hat & SDL_HAT_RIGHT)
                                                         ? 1
                                                         : 0;
                    } else { // down -> -1, centered -> 0, up -> 1, else -> 0
                        m_joystick.data()->hats[i] = (hat & SDL_HAT_DOWN) ? -1
                                                     : (hat & SDL_HAT_UP) ? 1
                                                                          : 0;
                    }
                }

                if (m_joystick.data()->buttons.ASFFast) {
                    for (size_t i = 0;
                         i < helpers::size(m_joystick.data()->runtimeASF); i++)
                        m_joystick.data()->runtimeASF[i] = 1.0f;
                }
                if (m_joystick.data()->buttons.ASFMedium) {
                    for (size_t i = 0;
                         i < helpers::size(m_joystick.data()->runtimeASF); i++)
                        m_joystick.data()->runtimeASF[i] = .75f;
                }
                if (m_joystick.data()->buttons.ASFSlow) {
                    for (size_t i = 0;
                         i < helpers::size(m_joystick.data()->runtimeASF); i++)
                        m_joystick.data()->runtimeASF[i] = .5f;
                }
                if (m_joystick.data()->buttons.ASFUltraSlow) {
                    for (size_t i = 0;
                         i < helpers::size(m_joystick.data()->runtimeASF); i++)
                        m_joystick.data()->runtimeASF[i] = .25f;
                }

                emit joystickUpdated(Joystick(m_joystick.data()));
            } else { // joystick exists but isn't opened
                m_sdlJoystick = SDL_JoystickOpen(0);
                qInfo() << "Connected joystick 0 ("
                        << SDL_JoystickName(m_sdlJoystick) << "), "
                        << SDL_JoystickNumAxes(m_sdlJoystick) << " axes, "
                        << SDL_JoystickNumButtons(m_sdlJoystick) << " buttons";
                joystickConnected = true;
                updateJoystick();
                updateSettings();
            }
        } else { // joystick exists but isn't opened
            m_sdlJoystick = SDL_JoystickOpen(0);
            qInfo() << "Connected joystick 0 ("
                    << SDL_JoystickName(m_sdlJoystick) << "),"
                    << SDL_JoystickNumAxes(m_sdlJoystick) << " axes,"
                    << SDL_JoystickNumButtons(m_sdlJoystick) << "buttons";
            joystickConnected = true;
            updateJoystick();
            updateSettings();
        }
    } else { // no joysticks connected
        if (!notifiedNoJoysticks) {
            qInfo() << "SDL cannot find joysticks on this system";
            emit joystickUpdated(Joystick());
            notifiedNoJoysticks = true;
            joystickConnected   = false;
        }
    }
}
