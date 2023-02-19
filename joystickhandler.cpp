#include "joystickhandler.h"

JoystickHandler::JoystickHandler(QObject *parent)
    : QObject{parent}
    , m_joystick(new Joystick)
    , m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "CfRD", "RovUI2", this))
{
    SDL_Init(SDL_INIT_JOYSTICK);
    SDL_JoystickUpdate();
    if(SDL_NumJoysticks()>0){
        m_joystick.data()->joystick = SDL_JoystickOpen(0);
        joystickConnected = true;
        doUpdateJoystick();
        doUpdateSettings();
    }
    startTimer(16);
}

void JoystickHandler::doUpdateSettings(){
    m_settings.data()->sync();
    for(int i = 0; i < 6; i++) //init axes from settings
        m_joystick.data()->axes_id[i] = m_settings->value(QString(SDL_JoystickName(m_joystick->joystick)) + QString("/joystickAxes/") + m_axesNames[i]).toInt();
    for(int i = 0; i < 16; i++) //same for buttons
        m_joystick.data()->buttons_id[i] = m_settings->value(QString(SDL_JoystickName(m_joystick->joystick)) + QString("/joystickButtons/") + m_buttonsNames[i]).toInt();
    for(int i = 0; i < 4; i++) //same for hats
        m_joystick.data()->hats_id[i] = m_settings->value(QString(SDL_JoystickName(m_joystick->joystick)) + QString("/joystickHats/") + m_hatsNames[i]).toInt();
}

void JoystickHandler::doUpdateJoystick(){
    m_joystick->numAxes = SDL_JoystickNumAxes(m_joystick->joystick);
    m_joystick->numHats = SDL_JoystickNumHats(m_joystick->joystick);
    m_joystick->numButtons = SDL_JoystickNumButtons(m_joystick->joystick);
    delete m_joystick->joystickName;
    m_joystick->joystickName = new QString(SDL_JoystickName(m_joystick->joystick));
    emit joystickChanged(Joystick(m_joystick.data()));
}

void JoystickHandler::timerEvent(QTimerEvent *){
    m_settings.data()->sync();
    SDL_JoystickUpdate();
    if(SDL_NumJoysticks()>0){
        if(m_joystick.data()->joystick != nullptr && joystickConnected == true){
            if(SDL_JoystickGetAttached(m_joystick.data()->joystick)){ //joystick should exist and be opened

                for(int i = 0; i < 6; i++){
                    float temp_axis = SDL_JoystickGetAxis(m_joystick.data()->joystick, m_joystick.data()->axes_id[i]);
                    float temp_axis_2 = m_joystick.data()->axes[i];
                    m_joystick.data()->axes[i] =
                            (
                             map(temp_axis, -32768, 32767, -100, 100) +
                             m_joystick.data()->axes[i] +
                             m_joystick.data()->axes_last[i]
                            ) / 3;
                    m_joystick.data()->axes_last[i] = temp_axis_2;
                }
                for(int i = 0; i < 16; i++){
                    if(SDL_JoystickGetButton(m_joystick.data()->joystick, m_joystick.data()->buttons_id[i]) == 1) //bit magic to set
                        m_joystick.data()->buttons |= 1UL << i;
                    if(SDL_JoystickGetButton(m_joystick.data()->joystick, m_joystick.data()->buttons_id[i]) == 0) //bit magic to clear
                        m_joystick.data()->buttons &= ~(1UL << i);
                }
                for(int i = 0; i < 4; i++){
                    m_joystick.data()->hats[i] = SDL_JoystickGetHat(m_joystick.data()->joystick, m_joystick.data()->hats_id[i]);
                }
                emit joystickUpdated(Joystick(m_joystick.data()));
            }else{ // joystick exists but isn't opened
                m_joystick.data()->joystick = SDL_JoystickOpen(0);
                qInfo() << "Connected joystick 0 (" << SDL_JoystickName(m_joystick.data()->joystick) <<
                           "), " << SDL_JoystickNumAxes(m_joystick.data()->joystick) <<
                           " axes, " << SDL_JoystickNumButtons(m_joystick.data()->joystick) << " buttons" << Qt::endl;
                joystickConnected = true;
                doUpdateJoystick();
                doUpdateSettings();
            }
        }else{ // joystick exists but isn't opened
            m_joystick.data()->joystick = SDL_JoystickOpen(0);
            qInfo() << "Connected joystick 0 (" << SDL_JoystickName(m_joystick.data()->joystick) <<
                       ")," << SDL_JoystickNumAxes(m_joystick.data()->joystick) <<
                       " axes," << SDL_JoystickNumButtons(m_joystick.data()->joystick) << "buttons" << Qt::endl;
            joystickConnected = true;
            doUpdateJoystick();
            doUpdateSettings();
        }
    }
    else{ // no joysticks connected
        if(!notifiedNoJoysticks){
            qInfo() << "SDL cannot find joysticks on this system" << Qt::endl;
            notifiedNoJoysticks = true;
            joystickConnected = false;
        }
    }
}
