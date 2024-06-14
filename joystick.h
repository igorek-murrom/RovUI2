#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QString>
#include <SDL_joystick.h>
#include <map>
/**
 * \namespace JoystickNames is used to standardize names and designations of the
 * axes and buttons and whatever
 */
namespace JoystickHelpers {
    /**
     * \brief Map of the axes' indices and their names
     * \image html rovAxes.png
     */
    enum axes { AxisX, AxisY, AxisZ, AxisW, AxisD, AxisR, AxisS };

    /**
     * \brief The buttons enum is used to ease access to buttons in Joystick
     * struct
     *
     * Button designations:\n
     * [0]: Open manipulator,\n
     * [1]: Close manipulator,\n
     * [2]: Rotate manipulator CW,\n
     * [3]: Rotate manipulator CCW,\n
     * [4]: Set ASFs to 1,\n
     * [5]: Set ASFs to 0.75,\n
     * [6]: Set ASFs to 0.5,\n
     * [7]: Set ASFs to 0.25,\n
     * [8]: Camera Select\n
     * [9-15]: Not used yet\n
     *
     * \see Joystick
     */
    enum buttons {
        ManipOpen,
        ManipClose,
        ManipCW,
        ManipCCW,
        ASFFast,
        ASFMedium,
        ASFSlow,
        ASFUltraSlow,
        CameraSelect,
        AddButton,
        DepthRegButtonON,
        DepthRegButtonOFF,
        Button13,
        Button14,
        Button15,
        Button16
    };

    /**
     * \var axesNames
     * \brief test
     */
    static QString axesNames[7] = {"AxisX", "AxisY", "AxisZ",
                                   "AxisW", "AxisD", "AxisR", "AxisS"};

    static QString buttonsNames[16] = {
        "ManipOpen", "ManipClose", "ManipCW",  "ManipCCW",
        "ASF-F",     "ASF-M",      "ASF-S",    "ASF-US",
        "CamSel",    "AddButton",   "DepthRegButtonON", "DepthRegButtonOFF",
        "Button13",  "Button14",   "Button15", "Button16"};

    static QString hatsNames[4] = {"CamServoFront", "CamServoRear", "Hat3",
                                   "Hat4"};
} // namespace JoystickHelpers


struct Axe {
    float axe;
    float axe_last;
    float id;
    float baseASF;
    float runtimeASF;
    int8_t direction;
};



/**
 * \brief The Joystick struct is used to help ordering of the data got from the
 * joystick \todo maybe split into JoystickData with axes, buttons and hats data
 * and JoystickInfo with name and other data describing the physical layout of
 * the joystick
 */
struct Joystick {

    Axe axis[7];

    /**
     * \var buttons
     * \brief Buttons of the joystick, compacted into uint16_t
     *
     * Mapped according to JoystickNames::buttons
     * \see JoystickNames::buttons
     */
    /**
     * \union _joystickButtons
     * \brief Buttons of the joystick, compacted into uint16_t
     *
     * Mapped according to JoystickNames::buttons
     * \see JoystickNames::buttons
     */
    union _joystickButtons {
        /**
         * \brief Raw buttons data
         */
        uint16_t rawData;
        struct {
            int8_t ManipOpen    : 1;
            int8_t ManipClose   : 1;
            int8_t ManipCW      : 1;
            int8_t ManipCCW     : 1;
            int8_t ASFFast      : 1;
            int8_t ASFMedium    : 1;
            int8_t ASFSlow      : 1;
            int8_t ASFUltraSlow : 1;
            int8_t CameraSelect : 1;
            int8_t AddButton    : 1;
            int8_t DepthRegButtonON : 1;
            int8_t DepthRegButtonOFF : 1;
            int8_t __res        : 3;
        };
    } buttons;

    /**
     * \brief Mapping of the buttons according to buttonsNames to the joystick
     * buttons \see JoystickNames::buttons
     */
    int8_t buttons_id[16];

    /**
     * \brief Hats' data from the joystick
     */
    int8_t hats[4];

    /**
     * \brief Mapping of the hats according to hatsNames to the joystick hats
     */
    int8_t hats_id[4];

    /**
     * \brief Determines which axis of the hat is used
     *
     *
     * Value  | Meaning
     * -------| -------------
     * true   | Horizontal axis is used (left is -1, right is +1)
     * false  | Vertical axis is used (down is -1, up is +1)
     */
    bool hats_hor[4];

    /**
     * \brief Name of the joystick, used for settings
     */
    QString joystickName;

    /**
     * \brief Number of axes>
     */
    int8_t numAxes;

    /**
     * \brief Number of buttons
     */
    int8_t numButtons;

    /**
     * \brief Number of hats
     */
    int8_t numHats;

    /**
     * \brief Default constructor
     */
    Joystick() {
        for (int i = 0; i < 7; i++) {
            this->axis[i].axe = 0;
            this->axis[i].axe_last = 0;
            this->axis[i].id = 0;
            this->axis[i].runtimeASF = 1;
            this->axis[i].baseASF = 1;
            this->axis[i].direction = 1;
        }
        this->buttons.rawData = 0;
        for (int i = 0; i < 16; i++) {
            buttons_id[i] = 0;
        }
        for (int i = 0; i < 4; i++) {
            this->hats[i]    = 0;
            this->hats_id[i] = 0;
        }
        this->numAxes      = 0;
        this->numButtons   = 0;
        this->numHats      = 0;
        this->joystickName = QString();
    }

    /**
     * \brief Copy-constructor
     * \param j Joystick
     */
    Joystick(Joystick *j) {
        for (int i = 0; i < 7; i++) {
            this->axis[i].axe = j->axis[i].axe;
            this->axis[i].axe_last = j->axis[i].axe_last;
            this->axis[i].id = j->axis[i].id;
            this->axis[i].runtimeASF = j->axis[i].runtimeASF;
            this->axis[i].baseASF = j->axis[i].baseASF;
            this->axis[i].direction = j->axis[i].direction;
        }

        for (int i = 0; i < 4; ++i) {
            this->hats[i]    = j->hats[i];
            this->hats_id[i] = j->hats_id[i];
        }
        this->buttons = j->buttons;
        for (int i = 0; i < 16; i++) {
            this->buttons_id[i] = j->buttons_id[i];
        }
        this->numAxes      = j->numAxes;
        this->numButtons   = j->numButtons;
        this->numHats      = j->numHats;
        this->joystickName = QString(j->joystickName);
    }
};

#endif // JOYSTICK_H
