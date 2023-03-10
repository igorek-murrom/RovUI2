#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QString>
#include <SDL_joystick.h>
#include <map>
namespace JoystickNames {
    static const std::map<int,QString> axesNames_map = {
        {1, "AxisX"},
        {2, "AxisY"},
        {3, "AxisZ"},
        {4, "AxisW"},
        {5, "AxisD"},
        {6, "AxisR"}
    };
    static QString axesNames[6] = { "AxisX", "AxisY", "AxisZ",
                                    "AxisW", "AxisD", "AxisR" };
    static QString buttonsNames[16] = {
                                       "ManipOpen", "ManipClose", "ManipCW",  "ManipCCW",
                                       "ASF-F",     "ASF-M",      "ASF-S",    "ASF-US",
                                       "CamSel",  "Button10",   "Button11", "Button12",
                                       "Button13",  "Button14",   "Button15", "Button16"};
    static QString hatsNames[4] = { "CamServo", "Hat2", "Hat3", "Hat4"};
}

struct Joystick
{
    SDL_Joystick *sdlJoystick;

    //TODO: setup baseASF
    //Axes designations:
    //[0]: Axis X,
    //[1]: Axis Y,
    //[2]: Axis Z,
    //[3]: Axis W,
    //[4]: Axis D,
    //[5]: Axis R
    float axes[6]; float axes_last[6]; int axes_id[6]; float asf[6]; float baseASF[6]; int directions[6];

    //Button designations:
    //[0]: Open manipulator,
    //[1]: Close manipulator,
    //[2]: Rotate manipulator CW,
    //[3]: Rotate manipulator CCW,
    //[4]: Set ASFs to 1,
    //[5]: Set ASFs to 0.75,
    //[6]: Set ASFs to 0.5,
    //[7]: Set ASFs to 0.25,
    //[8]: Camera Select
    //[9-15]: Not used yet
    uint16_t buttons; int buttons_id[16];

    //hats_hor is used to determine which axis of the hat is actually used
    //(true: horizontal axis is used, false: vertical axis is used)
    //left/down is -, up/right is +
    //Hats designations:
    //[0]: Cameras rotation,
    //[1-3]: Not used yet
    short hats[4]; short hats_id[4]; bool hats_hor[4];

    QString joystickName;

    short numAxes;
    short numButtons;
    short numHats;

    Joystick() {
        for(int i = 0; i < 6; i++){
            this->axes[i] = 0;
            this->axes_last[i] = 0;
            this->axes_id[i] = 0;
            this->asf[i] = 1;
            this->baseASF[i] = 1;
            this->directions[i] = 1;
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
        this->joystickName = QString();
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
        this->joystickName = QString(j->joystickName);
    }
};

#endif // JOYSTICK_H
