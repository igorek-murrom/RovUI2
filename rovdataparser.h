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
    void telemetryReady(RovTelemetry);
    void controlReady(QByteArray);
public slots:
    void doProcessTelemetry(QByteArray);
    void doPrepareDatagram(Joystick);
    void doThrustersOverride(bool);
    void doSetThrustersOverride(int[8]);
    void doSetThrustersOverrideInvert(int[8]);

private:
    constexpr static short thrusterDirections[10] = {1,-1,-1,1,-1,1,-1,1,1,1};
    int m_thrOvr[8] = {0,0,0,0,0,0,0,0};
    bool m_thrOvrInv[8] = {0,0,0,0,0,0,0,0};
    bool m_override = false;

    QMutex m_thrOvrMutex;
    QMutex m_thrOvrInvMutex;
    QMutex m_overrideMutex;

};

#endif // ROVDATAPARSER_H
