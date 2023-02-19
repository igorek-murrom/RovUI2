#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QString>
#include <SDL_joystick.h>

struct Joystick
{
    SDL_Joystick *joystick;
    //TODO: setup baseASF
    float axes[6]; float axes_last[6]; int axes_id[6]; float asf[6]; float baseASF[6]; int directions[6];//"AxisX", "AxisY", "AxisZ", "AxisW", "AxisD", "AxisR"
    uint16_t buttons; int buttons_id[16];//"ManipOpen", "ManipClose", "CamSel", "TSF-F", "TSF-M", "TSF-S", "Button7", "Button8", "Button9", "Button10", "Button11", "Button12", "Button13", "Button14", "Button15", "Button16"
    short hats[4]; int hats_id[4];//"CamServo", "Hat2", "Hat3", "Hat4"
    QString *joystickName;
    int numAxes;
    int numButtons;
    int numHats;

    Joystick() {
        for(int i = 0; i < 6; i++){
            this->axes[i] = 0;
            this->axes_last[i] = 0;
            this->axes_id[i] = 0;
            this->asf[i] = 0;
            this->baseASF[i] = 0;
            this->directions[i] = 0;
        }
        this->buttons = 0;
        for (int i = 0; i < 16; i++) {
            buttons_id[i] = 0;
        }
        for (int i = 0; i < 4; i++) {
            this->hats[i] = 0;
            this->hats_id[i] = 0;
        }
        this->numAxes = 0;
        this->numButtons = 0;
        this->numHats = 0;
        this->joystickName = new QString;
    }
    Joystick(Joystick *j){
        for(int i = 0; i < 6; i++){
            this->axes[i] = j->axes[i];
            this->axes_last[i] = j->axes_last[i];
            this->axes_id[i] = j->axes_id[i];
            this->asf[i] = j->asf[i];
            this->baseASF[i] = j->baseASF[i];
            this->directions[i] = j->directions[i];
        }
        for (int i = 0; i < 4; ++i) {
            this->hats[i] = j->hats[i];
            this->hats_id[i] = j->hats_id[i];
        }
        this->buttons = j->buttons;
        for(int i = 0; i < 16; i++){
            this->buttons_id[i] = j->buttons_id[i];
        }
        this->numAxes = j->numAxes;
        this->numButtons = j->numButtons;
        this->numHats = j->numHats;
        this->joystickName = new QString(*j->joystickName);
    }
};

#endif // JOYSTICK_H
