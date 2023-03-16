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
public slots:
    void processTelemetry(QByteArray);
    void prepareControl(Joystick);
    void prepareAuxControl();
    void enableThrustersOverride(bool);
    void setThrustersOverride(QList<qint8>);
    void setThrustersOverrideInvert(qint8);
    void setAuxFlags(qint8);


private:
    constexpr static short thrusterDirections[10] = {1,-1,-1,1,-1,1,-1,1,1,1};
    QList<qint8> m_thrOvr = {0,0,0,0,0,0,0,0};
    qint8 m_thrOvrInv = 0b00000000;
    QList<qint8> m_desiredRegVals = {0,0,0,0};
    qint8 m_auxFlags = 0b00000000;
    bool m_override = false;

    QScopedPointer<RovControlDatagram> m_datagram;

    QMutex m_thrOvrMutex;
    QMutex m_thrOvrInvMutex;
    QMutex m_overrideMutex;

};

#endif // ROVDATAPARSER_H
