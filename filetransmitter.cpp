#include "filetransmitter.h"
#include "qdebug.h"
#include "qobjectdefs.h"
#include "qthread.h"

FileTransmitter::FileTransmitter(QObject *parent)
    : QTcpServer{parent}, clients() {
    listen(QHostAddress::Any, m_port);
    qDebug() << "Listening @ " << serverAddress() << ":" << serverPort();
    qDebug() << "Server status is " << this->isListening() << "("
             << errorString() << ")";
}

void FileTransmitter::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *sock = new QTcpSocket(0);
    sock->setSocketDescriptor(socketDescriptor);
    qDebug() << "Got incoming connection from " << sock->peerAddress();
    Host host = Host(sock, QHostInfo::fromName(sock->peerName()));
    qDebug() << "SD is " << socketDescriptor;
    clients.append(Host(host));
    emit newHost(clients.size() - 1);
}

void FileTransmitter::sendFileAt(QString path, Host host) {
    QTcpSocket *socket = host.socket;
    QDataStream dataStream;
    qDebug() << "SD is " << host.socket->socketDescriptor();

    dataStream.setDevice(socket);
    dataStream.setVersion(QDataStream::Qt_5_9);

    qDebug() << "Sending file to " << host.hostInfo.hostName() << "("
             << host.hostInfo.addresses() << ")";

    QFile file(path);

    qDebug() << "initted streams";

    dataStream << 0xAA; // send size packet
    dataStream << file.size();
    dataStream << file.fileName();

    qDebug() << "writing size (" << file.size() << "b)";
    socket->waitForBytesWritten();

    qDebug() << "reading ACK";
    socket->waitForReadyRead(); // read ack packet

    uint header = 0;
    qDebug() << "reading header";
    dataStream >> header;

    if (header != 0xBA) {
        qDebug() << "Header is shit" << QString::number(header, 16);
        // qDebug() << dataStream;
        return;
    }
    do {
        dataStream << 0xAB;
        QByteArray ba;
        file.read(ba.data(), 32768);
        dataStream << ba;
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        dataStream >> header;
    } while (!file.atEnd());
    dataStream << 0xAC;
    socket->waitForBytesWritten();

    socket->waitForReadyRead();
    dataStream >> header;
};