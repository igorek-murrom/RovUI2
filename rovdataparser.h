#ifndef ROVDATAPARSER_H
#define ROVDATAPARSER_H

#include "joystick.h"
#include "qelapsedtimer.h"
#include "qwidget.h"
#include "rovdatatypes.h"
#include <QDataStream>
#include <QDebug>
#include <QMutex>
#include <QObject>
/**
 * @brief Floating point PD regulator implementation
 *
 */
class FPPDRegulator {
  public:
    /**
     * @brief Construct a new FPPDRegulator object
     *
     * @param coeffP Proportional coefficient
     * @param coeffD Differential coefficient
     */
    FPPDRegulator(float coeffP, float coeffD, float coeffI)
        : kP(coeffP), kD(coeffD), kI(coeffI), lastError(0.0f), lastTime(0){};
    /**
     * @brief Evaluate the regulator expression
     *
     * @param data Data
     * @param target Target
     * @return float Control signal
     */
    float eval(float data, bool isYaw = false);
    void setTarget(float target);
    void  enable();
    void  disable();
    bool enabled = false;
    float uP = 0, uD = 0, uI = 0;

  private:
    float kP = 0;
    float kI = 0;
    float kD = 0;
    float error = 0;
    float lastError;
    float target;

    int      lastData;
    uint32_t lastTime;

    QElapsedTimer eTimer;
};


/**
 * \brief The RovDataParser class is responsible for packing control data in
 * QByteArrays and unpacking telemetry data from them.
 */
class RovDataParser : public QWidget {
    Q_OBJECT
  public:
    /**
     * \brief Default constructor
     * \param parent Parent
     */
    explicit RovDataParser(QWidget *parent = nullptr);
    QScopedPointer<RovControl> m_control;


  signals:
    /**
     * \brief Emitted when the class is finished processing raw QByteArray
     * \param tele Telemetry object
     */
    void telemetryProcessed(RovTelemetry tele);

    /**
     * \brief Emitted when the class is finished packing RovControl struct into
     * a QByteArray \param ba QByteArray with the control data
     */
    void controlReady(QByteArray ba);

    /**
     * \brief Emitted when the class is finished packing RovAuxControl struct
     * into a QByteArray \param ba QByteArray with the auxiliary data
     */
    void auxControlReady(QByteArray ba);
  public slots:

    /**
     * \brief Called to unpack telemetry data from the QByteArray into the
     * RovTelemetry struct \param ba Datagrem with telemetry data
     */
    void processTelemetry(QByteArray ba);

    /**
     * \brief Called to pack joystick data from Joystick struct and then into
     * QByteArray \param joy Joystick struct with data
     */
    void prepareControl(Joystick joy);

    /**
     * \brief Called to pack the auxiliary control data into QByteArray
     */
    void prepareAuxControl();

    /**
     * \brief Enables or disables thruster override status
     * \param override Status
     */
    void enableThrustersOverride(bool override);

    /**
     * \brief Sets thrusters' overrides values
     * \param tOverride List with values
     */
    void setThrustersOverride(QList<qint8> tOverride);
    /**
     * \brief Sets thrusters' invert values
     * \param invert Packed invert values
     */
    void setThrustersOverrideInvert(qint8 invert);

    /**
     * \brief Sets RovAuxControl' auxiliary flags
     * \param aFlags Flags
     */
    void setAuxFlags(qint8 aFlags);

    /**
     * \brief Enables or disables depth regulator
     * \param status
     */
    void setDepthStatus(int status);
    /**
     * \brief Sets desired depth
     * \param dVal Desired value
     */
    void setDepth(double dVal);

    /**
     * \brief Enables or disables depth regulator
     * \param status
     */
    void setYawStatus(int status);
    /**
     * \brief Sets desired yaw
     * \param dVal Desired value
     */
    void setYaw(double dVal);

    /**
     * \brief Enables or disables depth regulator
     * \param status
     */
    void setRollStatus(int status);
    /**
     * \brief Sets desired roll
     * \param dVal Desired value
     */
    void setRoll(double dVal);

    /**
     * \brief Enables or disables depth regulator
     * \param status
     */
    void setPitchStatus(int status);
    /**
     * \brief Sets desired pitch
     * \param dVal Desired value
     */
    void setPitch(double dVal);

    void toggleLight();

    void togglePump();

    void invalidateImuCalibration();

  private:
    /**
     * \brief Thruster override values
     */
    QList<qint8> m_thrOvr = {0, 0, 0, 0, 0, 0, 0, 0};
    /**
     * \brief Mutex for m_thrOvr
     */
    QMutex       m_thrOvrMutex;

    /**
     * \brief Packed invert values for thrusters
     */
    qint8  m_thrOvrInv = 0b00000000;
    /**
     * \brief Mutex for m_thrOvrInv
     */
    QMutex m_thrOvrInvMutex;

    /**
     * \brief Status of the override
     */
    bool   m_thrOvrEnable;
    /**
     * \brief Mutex for m_thrOvrEnable
     */
    QMutex m_thrOvrEnableMutex;

    /**
     * \brief Internally-stored RovControl struct
     */
    /**
     * \brief Mutex for m_control
     */
    QMutex                     m_controlMutex;

    /**
     * \brief Previous CamSel state, used to switch cameras on the ROV
     */
    bool m_prevCamSelState;

    /**
     * \brief Internally-stored RovAuxControl struct
     */
    QScopedPointer<RovAuxControl> m_auxControl;

    /**
     * \brief Mutex for m_auxControl
     */
    QMutex m_auxControlMutex;

    RovTelemetry m_tele;

    FPPDRegulator depthReg;
    FPPDRegulator yawReg;
    FPPDRegulator rollReg;
    FPPDRegulator pitchReg;
    bool isFirstParsing = true;
    float depthEtalon = 0;
    float yawEtalon = 0;
    float rollEtalon = 0, pitchEtalon = 0;

};

#endif // ROVDATAPARSER_H
