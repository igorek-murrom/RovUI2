#ifndef ROVDATATYPES_H
#define ROVDATATYPES_H

#include "qfloat16.h"
#include <QtGlobal>

/*!
 * \brief The RovControl struct is used for ordering of the data used by RovUI to control the ROV
 */
struct RovControl
{
    /*!
     * \brief Header of the packet, used for distinguishing it from other types of packets
     */
    const static qint8 header = 0xAC;
    /*!
     * \brief Version of the protocol used by the RovUI and ROV to communicate
     * \todo Implement handshake protocol
     */
    quint8 version = 2;
    /*!
     * \brief Thrusters power to be set on the ROV
     */
    qint8 thrusterPower[10] = {0,0,0,0,0,0,0,0,0,0};
    /*!
     * \brief Pending camera rotation (<0 is down, >0 is up): 1st is for the front camera, 2nd is for the rear one
     */
    qint8 cameraRotationDelta[2] = {0,0};
    /*!
     * \brief Pending manipulator action (<0 is open, >0 is close)
     */
    qint8 manipulatorOpenClose = 0;
    /*!
     * \brief Pending manipultor action (<0 is CCW, >0 is CW)
     */
    qint8 manipulatorRotate = 0;
    /*!
     * \brief Camera select variable (false is front camera, true is back camera)
     */
    qint8 camsel = false;
    /*!
     * \brief Default constructor
     */
    RovControl(){}
};

/*!
 * \brief The RovAuxControl struct is used for controlling the regulators data used by RovUI to control the ROV
 */
struct RovAuxControl
{
    /*!
     * \brief Header of the packet, used for distinguishing it from other types of packets
     */
    const static qint8 header = 0xAD;
    /*!
     * \brief Auxilary flags, used to control the regulators, and possibly something else
     */
    qint8 auxFlags = 0b00000000;
    /*!
     * \brief Desired depth
     */
    float dDepth = 0;
    /*!
     * \brief Desired yaw
     */
    float dYaw = 0;
    /*!
     * \brief Desired roll
     */
    float dRoll = 0;
    /*!
     * \brief Desired pitch
     */
    float dPitch = 0;
    /*!
     * \brief Default constructor
     */
    RovAuxControl(){};
};

/*!
 * \brief The RovHeartBeat struct is used for ordering of the data with the heartbeat signals coming from the ROV
 */
struct RovHeartBeat
{
    /*!
     * \brief Header of the packet, used for distinguishing it from other types of packets
     */
    const static qint8 header = 0xAF;

    /*!
     * \brief Milliseconds from the start of the ROV
     */
    quint64 millis = 0;
    /*!
     * \brief Sequence number, used to detect possible network failures
     */
    qint8 seqNumber = 0;
};

/*!
 * \brief The RovTelemetry struct is used for ordering of the data with the telemetry coming from the ROV
 */
struct RovTelemetry
{
    /*!
     * \brief Header of the packet, used for distinguishing it from other types of packets
     */
    const static qint8 header = 0xAE;
    /*!
     * \brief Version of the protocol used by the RovUI and ROV to communicate
     * \todo Implement handshake protocol
     */
    qint8 version = 2;
    /*!
     * \brief Depth data from the ROV
     */
    float depth = 0.0f;
    /*!
     * \brief Pitch angle data from the ROV
     * Valid values are from -180 to 180, in degrees
     * \image html rov_pitch_img.png
     */
    float pitch = 0;
    /*!
     * \brief Yaw angle data from the ROV
     * Valid values are from 0 to 360, in degrees
     * \image html rov_yaw_img.png
     */
    float yaw = 0;
    /*!
     * \brief Roll angle data from the ROV
     * Valid values are from -180 to 180, in degrees
     * \image html rov_roll_img.png
     */
    float roll = 0;
    /*!
     * \brief Current consumption data from the ROV, in A
     * \b {SHOULD BE LESS THAN 25A}
     */
    float current = 0.0f;
    /*!
     * \brief Voltage coming to the ROV, in V
     * \b {SHOULD BE NO LESS THAN 6V AND NO MORE THAN 12V}
     */
    float voltage = 0.0f;
    /*!
     * \see RovControl.camsel
     */
    qint8 cameraIndex = 0;
    /*!
     * \brief Temperature data from the ROV
     * Temperature sensor is located outside the electronics enclosure
     */
    float temperature = 0.0f;
    RovTelemetry() {}
};



#endif // ROVDATATYPES_H
