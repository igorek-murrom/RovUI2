#ifndef JOYSTICKHANDLER_H
#define JOYSTICKHANDLER_H

#include "helpers.h"
#include "joystick.h"
#include "qmutex.h"
#include <QDebug>
#include <QObject>
#include <QSettings>
#include <QTimer>
#include <SDL.h>
#include <SDL_joystick.h>

/*!
 * \brief The JoystickHandler class is used to parse input from the connected
 * joystick(-s)
 */
class JoystickHandler : public QObject {
    Q_OBJECT
  public:
    /*!
     * \brief Default constructor
     * \param parent Parent
     */
    explicit JoystickHandler(QObject *parent = nullptr);

  signals:
    /*!
     * \brief Emitted on every data change, i.e. button press, axis move etc.
     */
    void joystickUpdated(Joystick);

    /*!
     * \brief Emitted on every joystick change, i.e. plug/unplug, etc.
     */
    void joystickChanged(Joystick);

  public slots:

    /*!
     * \brief Called to update settings from settings files
     */
    void updateSettings();

    /*!
     * \brief Called to update everything about a connected joystick, including
     * settings
     */
    void updateJoystick();

    /*!
     * \brief Called to update ASFs
     * \param asf Array of ASFs according to JoystickNames::axes
     */
    void updateASFs(int asf[6]);
  private slots:
    /*!
     * \brief "main loop" of handler
     */
    void timerEvent(QTimerEvent *);

  private:
    /*!
     * \brief Maps value x from range defined by in_min and in_max to range
     * defined by out_min and out_max \param x \param in_min \param in_max
     * \param out_min \param out_max \return Mapped value
     */
    inline float map(float x, float in_min, float in_max, float out_min,
                     float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    /*!
     * \brief Mutex which is used to prevent data race conditions related to
     * m_joystick
     */
    QMutex m_joystickMutex;

    /*!
     * \brief Pointer to a Joystick struct
     */
    QScopedPointer<Joystick> m_joystick;

    /*!
     * \brief SDL representation of the joystick
     */
    SDL_Joystick *m_sdlJoystick;

    /*!
     * \brief Settings object, used to retrieve and store - wow - settings
     */
    QScopedPointer<QSettings> m_settings;

    /*!
     * \brief Flag used to track user notification status
     */
    bool notifiedNoJoysticks = false;
    /*!
     * \brief Flag used to track user notification status
     */
    bool notifiedJoystickDisconnected = false;
    /*!
     * \brief Flag used to track joystick connection
     */
    bool joystickConnected = false;
};

#endif // JOYSTICKHANDLER_H
