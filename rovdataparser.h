#ifndef ROVDATAPARSER_H
#define ROVDATAPARSER_H

#include <QObject>
#include <QDebug>
#include <QDataStream>
#include <QMutex>
#include <algorithm>
#include "joystick.h"
#include "rovdatatypes.h"
/*!
 * \brief The RovDataParser class is responsible for packing control data in QByteArrays and unpacking telemetry data from them.
 */
class RovDataParser : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Default constructor
     * \param parent Parent
     */
    explicit RovDataParser(QObject *parent = nullptr);

signals:
    /*!
     * \brief Emitted when the class is finished processing raw QByteArray
     * \param tele Telemetry object
     */
    void telemetryProcessed(RovTelemetry tele);

    /*!
     * \brief Emitted when the class is finished packing RovControl struct into a QByteArray
     * \param ba QByteArray with the control data
     */
    void controlReady(QByteArray ba);

    /*!
     * \brief Emitted when the class is finished packing RovAuxControl struct into a QByteArray
     * \param ba QByteArray with the auxiliary data
     */
    void auxControlReady(QByteArray ba);
public slots:

    /*!
     * \brief Called to unpack telemetry data from the QByteArray into the RovTelemetry struct
     * \param ba Datagrem with telemetry data
     */
    void processTelemetry(QByteArray ba);

    /*!
     * \brief Called to pack joystick data from Joystick struct and then into QByteArray
     * \param joy Joystick struct with data
     */
    void prepareControl(Joystick joy);

    /*!
     * \brief Called to pack the auxiliary control data into QByteArray
     */
    void prepareAuxControl();

    /*!
     * \brief Enables or disables thruster override status
     * \param override Status
     */
    void enableThrustersOverride(bool override);

    /*!
     * \brief Sets thrusters' overrides values
     * \param tOverride List with values
     */
    void setThrustersOverride(QList<qint8> tOverride);
    /*!
     * \brief Sets thrusters' invert values
     * \param invert Packed invert values
     */
    void setThrustersOverrideInvert(qint8 invert);

    /*!
     * \brief Sets RovAuxControl' auxiliary flags
     * \param aFlags Flags
     */
    void setAuxFlags(qint8 aFlags);

    /*!
     * \brief Sets desired depth
     * \param dVal Desired value
     */
    void setDepth(double dVal);

    /*!
     * \brief Sets desired yaw
     * \param dVal Desired value
     */
    void setYaw(double dVal);

    /*!
     * \brief Sets desired roll
     * \param dVal Desired value
     */
    void setRoll(double dVal);

    /*!
     * \brief Sets desired pitch
     * \param dVal Desired value
     */
    void setPitch(double dVal);


private:
    /*!
     * \brief Thruster directions. Thrusters' values are multiplied by these in prepareControl
     */
    constexpr static short thrusterDirections[10] = {1,-1,-1,1,-1,1,-1,1,1,1};

    /*!
     * \brief Thruster override values
     */
    QList<qint8> m_thrOvr = {0,0,0,0,0,0,0,0};
    /*!
     * \brief Mutex for m_thrOvr
     */
    QMutex m_thrOvrMutex;

    /*!
     * \brief Packed invert values for thrusters
     */
    qint8 m_thrOvrInv = 0b00000000;
    /*!
     * \brief Mutex for m_thrOvrInv
     */
    QMutex m_thrOvrInvMutex;

    /*!
     * \brief Status of the override
     */
    bool m_thrOvrEnable;
    /*!
     * \brief Mutex for m_thrOvrEnable
     */
    QMutex m_thrOvrEnableMutex;

    /*!
     * \brief Internally-stored RovControl struct
     */
    QScopedPointer<RovControl> m_control;
    /*!
     * \brief Mutex for m_control
     */
    QMutex m_controlMutex;

    /*!
     * \brief Previous CamSel state, used to switch cameras on the ROV
     */
    bool m_prevCamSelState;

    /*!
     * \brief Internally-stored RovAuxControl struct
     */
    QScopedPointer<RovAuxControl> m_auxControl;

    /*!
     * \brief Mutex for m_auxControl
     */
    QMutex m_auxControlMutex;


};

#endif // ROVDATAPARSER_H
