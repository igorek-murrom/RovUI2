#include "joystickhandler.h"

JoystickHandler::JoystickHandler(QObject *parent)
    : QObject{parent}
    , m_joystick(new Joystick)
    , m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "CfRD", "RovUI2", this))
{
    SDL_Init(SDL_INIT_JOYSTICK);
    SDL_JoystickUpdate();
    if(SDL_NumJoysticks()>0){
        m_joystick.data()->sdlJoystick = SDL_JoystickOpen(0);
        joystickConnected = true;
        doUpdateJoystick();
        doUpdateSettings();
    }
    startTimer(16);
}

void JoystickHandler::doUpdateSettings(){
    m_settings.data()->sync();
    for(int i = 0; i < 6; i++)  //init axes from settings
        m_joystick.data()->axes_id[i] = m_settings->value(QString(SDL_JoystickName(m_joystick->sdlJoystick)) + QString("/joystickAxes/") + JoystickNames::axesNames[i]).toInt();
    for(int i = 0; i < 16; i++) //same for buttons
        m_joystick.data()->buttons_id[i] = m_settings->value(QString(SDL_JoystickName(m_joystick->sdlJoystick)) + QString("/joystickButtons/") + JoystickNames::buttonsNames[i]).toInt();
    for(int i = 0; i < 4; i++)  //same for hats
        m_joystick.data()->hats_id[i] = m_settings->value(QString(SDL_JoystickName(m_joystick->sdlJoystick)) + QString("/joystickHats/") + JoystickNames::hatsNames[i]).toInt();
}

void JoystickHandler::doUpdateJoystick(){
    m_joystick->numAxes = SDL_JoystickNumAxes(m_joystick->sdlJoystick);
    m_joystick->numHats = SDL_JoystickNumHats(m_joystick->sdlJoystick);
    m_joystick->numButtons = SDL_JoystickNumButtons(m_joystick->sdlJoystick);
    m_joystick->joystickName = QString(SDL_JoystickName(m_joystick->sdlJoystick));
    emit joystickChanged(Joystick(m_joystick.data()));
}

void JoystickHandler::doUpdateASFs(int asf[6]){
    for (int i = 0; i < 8; ++i) {
        m_joystick->asf[i] = asf[i];
    }
}

void JoystickHandler::timerEvent(QTimerEvent *){
    m_settings.data()->sync();
    SDL_JoystickUpdate();
    if(SDL_NumJoysticks()>0){
        if(m_joystick.data()->sdlJoystick != nullptr && joystickConnected == true){
            if(SDL_JoystickGetAttached(m_joystick.data()->sdlJoystick)){ //joystick should exist and be opened

                for(int i = 0; i < 6; i++){ // joystick input smoothing 3x
                    float joystickAxisVal = SDL_JoystickGetAxis(m_joystick.data()->sdlJoystick, m_joystick.data()->axes_id[i]);
                    float lastAxisVal = m_joystick.data()->axes[i];
                    float secLastAxisVal = m_joystick.data()->axes_last[i];

                    m_joystick.data()->axes[i] = (map(joystickAxisVal, -32768, 32767, -101, 101) + lastAxisVal + secLastAxisVal) / 3;
                    m_joystick.data()->axes_last[i] = lastAxisVal;
                }

                for(int i = 0; i < 16; i++){ // write buttons into the variable according to buttonsNames
                    if(SDL_JoystickGetButton(m_joystick.data()->sdlJoystick, m_joystick.data()->buttons_id[i]))
                        BIT_SET(m_joystick.data()->buttons, i);
                    else
                        BIT_CLEAR(m_joystick.data()->buttons, i);
                }

                for(int i = 0; i < 4; i++){ // write hats

                    int hat = SDL_JoystickGetHat(m_joystick.data()->sdlJoystick, m_joystick.data()->hats_id[i]);
                    if(m_joystick.data()->hats_hor[i]) // left -> -1, centered -> 0, right -> 1, else -> 0
                        m_joystick.data()->hats[i] = (hat & SDL_HAT_LEFT) ? -1 : (hat & SDL_HAT_CENTERED) ? 0 : (hat & SDL_HAT_RIGHT) ? 1 : 0;

                    else                               // down -> -1, centered -> 0, up -> 1, else -> 0
                        m_joystick.data()->hats[i] = (hat & SDL_HAT_DOWN) ? -1 : (hat & SDL_HAT_CENTERED) ? 0 : (hat & SDL_HAT_UP) ? 1 : 0;
                }

                emit joystickUpdated(Joystick(m_joystick.data()));

            }else{ // joystick exists but isn't opened
                m_joystick.data()->sdlJoystick = SDL_JoystickOpen(0);
                qInfo() << "Connected joystick 0 (" << SDL_JoystickName(m_joystick.data()->sdlJoystick) <<
                           "), " << SDL_JoystickNumAxes(m_joystick.data()->sdlJoystick) <<
                           " axes, " << SDL_JoystickNumButtons(m_joystick.data()->sdlJoystick) << " buttons" << Qt::endl;
                joystickConnected = true;
                doUpdateJoystick();
                doUpdateSettings();
            }
        }else{ // joystick exists but isn't opened
            m_joystick.data()->sdlJoystick = SDL_JoystickOpen(0);
            qInfo() << "Connected joystick 0 (" << SDL_JoystickName(m_joystick.data()->sdlJoystick) <<
                       ")," << SDL_JoystickNumAxes(m_joystick.data()->sdlJoystick) <<
                       " axes," << SDL_JoystickNumButtons(m_joystick.data()->sdlJoystick) << "buttons" << Qt::endl;
            joystickConnected = true;
            doUpdateJoystick();
            doUpdateSettings();
        }
    }
    else{ // no joysticks connected
        if(!notifiedNoJoysticks){
            qInfo() << "SDL cannot find joysticks on this system" << Qt::endl;
            emit joystickUpdated(Joystick());
            notifiedNoJoysticks = true;
            joystickConnected = false;
        }
    }
}
