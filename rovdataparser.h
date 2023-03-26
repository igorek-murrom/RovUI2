#ifndef ROVDATAPARSER_H
#define ROVDATAPARSER_H

#include <QObject>
#include <QDebug>
#include <QDataStream>
#include <QMutex>
#include <algorithm>
#include "joystick.h"
#include "rovdatatypes.h"

class RovDataParser : public QObject
{
    Q_OBJECT
public:
    explicit RovDataParser(QObject *parent = nullptr);

signals:
    void telemetryProcessed(RovTelemetry);

    void controlReady(QByteArray);
    void auxControlReady(QByteArray);
public slots:
    void processTelemetry(QByteArray);

    void prepareControl(Joystick);
    void prepareAuxControl();

    void enableThrustersOverride(bool);
    void setThrustersOverride(QList<qint8>);
    void setThrustersOverrideInvert(qint8);

    void setAuxFlags(qint8);
    void setDepth(double);
    void setYaw(double);
    void setRoll(double);
    void setPitch(double);


private:
    constexpr static short thrusterDirections[10] = {1,-1,-1,1,-1,1,-1,1,1,1};
    QList<qint8> m_thrOvr = {0,0,0,0,0,0,0,0};
    QMutex m_thrOvrMutex;

    qint8 m_thrOvrInv = 0b00000000;
    QMutex m_thrOvrInvMutex;

    bool m_thrOvrEnable;
    QMutex m_thrOvrEnableMutex;

    QScopedPointer<RovControlDatagram> m_datagram;
    QMutex m_datagramMutex;

    bool m_prevCamSelState;

    QScopedPointer<RovAuxDatagram> m_auxDatagram;
    QMutex m_auxDatagramMutex;


};

#endif // ROVDATAPARSER_H
