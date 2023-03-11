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
    void doProcessTelemetry(QByteArray);
    void doPrepareDatagram(Joystick);
    void doEnableThrustersOverride(bool);
    void doSetThrustersOverride(QList<qint8>);
    void doSetThrustersOverrideInvert(qint8);

private:
    constexpr static short thrusterDirections[10] = {1,-1,-1,1,-1,1,-1,1,1,1};
    QList<qint8> m_thrOvr = {0,0,0,0,0,0,0,0};
    qint8 m_thrOvrInv = 0b00000000;
    bool m_override = false;

    QScopedPointer<RovDatagram> m_datagram;

    QMutex m_thrOvrMutex;
    QMutex m_thrOvrInvMutex;
    QMutex m_overrideMutex;

};

#endif // ROVDATAPARSER_H
