#ifndef ROVCOMMUNICATION_H
#define ROVCOMMUNICATION_H

#include <QObject>
#include <QtNetwork>

class RovCommunication : public QObject
{
    Q_OBJECT
public:
    explicit RovCommunication(QObject *parent = nullptr);

signals:
    void datagramRead(QByteArray);
public slots:

private slots:
    void doReadTelemetry();

private:
    quint16 m_inPort = 3010;
    QHostAddress m_rovAddress = QHostAddress("192.168.1.5");
    quint16 m_rovPort = 3020;
    QScopedPointer<QUdpSocket> m_udpSocket;
};

#endif // ROVCOMMUNICATION_H
