#include "rovcommunication.h"

RovCommunication::RovCommunication(QObject *parent)
    : QObject{parent}
    , m_udpSocket(new QUdpSocket(this))
{
    m_udpSocket.data()->bind(m_rovAddress, m_rovPort);
    connect(m_udpSocket.data(), &QUdpSocket::readyRead,
                this, &RovCommunication::doReadTelemetry);

}

void RovCommunication::doReadTelemetry(){
    while (m_udpSocket.data()->hasPendingDatagrams()) {
        QByteArray datagram = m_udpSocket->receiveDatagram().data();
        emit datagramRead(QByteArray(datagram));
    }
}
