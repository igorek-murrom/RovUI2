#include "rovcommunication.h"

RovCommunication::RovCommunication(QObject *parent)
    : QObject{parent}, m_udpSocket(new QUdpSocket(this)) {
    m_udpSocket.data()->bind(m_rcPort);
    connect(m_udpSocket.data(), &QUdpSocket::readyRead, this,
            &RovCommunication::read);
}

void RovCommunication::read() {
    while (m_udpSocket.data()->hasPendingDatagrams()) {
        emit telemetryReady(QByteArray(m_udpSocket->receiveDatagram().data()));
    }
}
void RovCommunication::write(QByteArray datagram) {
    m_udpSocket->writeDatagram(datagram, m_rovAddress, m_rovPort);
}
