#ifndef ROVCOMMUNICATION_H
#define ROVCOMMUNICATION_H

#include <QObject>
#include <QtNetwork>
#include "rovdatatypes.h"

class RovCommunication : public QObject
{
    Q_OBJECT
public:
    explicit RovCommunication(QObject *parent = nullptr);

signals:
    void telemetryReady(QByteArray);
    void telemetryStopped();
    void telemetryStarted();
public slots:
    void write(QByteArray);
private slots:
    void read();

private:
    quint16 m_inPort = 3010;
    QHostAddress m_rovAddress = QHostAddress("192.168.1.5");
    quint16 m_rovPort = 3010;

    QScopedPointer<QUdpSocket> m_udpSocket;
};

#endif // ROVCOMMUNICATION_H
