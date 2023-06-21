#include "filetransmitter.h"
#include "qdebug.h"
#include "qhostaddress.h"
#include "qobjectdefs.h"
#include "qsctpserver.h"
#include "qsctpsocket.h"
#include "qthread.h"
#include "qtimer.h"
#include <cstddef>
#include <cstdlib>

FileTransmitter::FileTransmitter(QObject *parent)
    : QTcpServer{parent}, clients(), m_networkThread(new FileWorkerThread()) {
    listen(QHostAddress::Any, m_port);
    qDebug() << "Listening @ " << serverAddress() << ":" << serverPort();
    qDebug() << "Server status is " << this->isListening() << "("
             << errorString() << ")";

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        size_t i = 0;
        for (Host host : clients) {
            if (host.socket->openMode() == QTcpSocket::NotOpen ||
                host.socket->state() == QTcpSocket::UnconnectedState) {
                qDebug() << "Removing host in server";
                emit removeHost(i);
                clients.removeAt(i);
            }
            i++;
        }
    });
    timer->start(100);
}

void FileTransmitter::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *sock = new QTcpSocket(0);
    sock->setSocketDescriptor(socketDescriptor);
    qDebug() << "Got incoming connection from " << sock->peerName() << " ("
             << sock->peerAddress() << ")";
    Host host = Host(sock, QHostInfo::fromName(sock->peerName()));
    qDebug() << "SD is " << socketDescriptor;
    clients.append(Host(host));
    emit newHost(clients.size() - 1);
}

void FileWorkerThread::run() {
    QDataStream stream;
    stream.setDevice(pendingHost.socket);
    stream.setVersion(QDataStream::Qt_5_15);

    qDebug() << "------------Sending file to " << pendingHost.socket->peerName()
             << "(" << pendingHost.socket->peerAddress() << ")------------";
    uint header;

    QFile file(pendingPath);
    file.open(QIODevice::ReadOnly);
    qDebug() << file.errorString();

    stream << HeaderBeginOut;
    stream << (qint64)file.size();
    pendingHost.socket->waitForBytesWritten();

    pendingHost.socket->waitForReadyRead();
    stream >> header;

    if (header != HeaderRBeginIn) {
        qDebug() << QString(
                        "Wrong HeaderRBeginIn (got %1, expected %2), exiting")
                        .arg(header)
                        .arg(HeaderRBeginIn);
        stream << HeaderEndOut;
        return;
    }

    QByteArray fileba;
    do {
        stream << HeaderFileOut;

        fileba = file.read(DATAGRAM_SIZE);
        stream << fileba;

        pendingHost.socket->waitForReadyRead();

        stream >> header;

        if (header != HeaderRFileIn) {
            qDebug()
                << QString("Wrong HeaderRFileIn (got %1, expected %2), exiting")
                       .arg(header)
                       .arg(HeaderRFileIn);
            return;
        }

    } while (!file.atEnd());
    stream << HeaderEndOut;
    stream >> header;
    if (header != HeaderREndIn) {
        qDebug() << QString("Wrong HeaderREndIn (got %1, expected %2), exiting")
                        .arg(header)
                        .arg(HeaderREndIn);
        return;
    }
}