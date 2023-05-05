#include "filetransmitter.h"
#include "qdebug.h"
#include "qobjectdefs.h"
#include "qsctpserver.h"
#include "qsctpsocket.h"
#include "qthread.h"
#include <cstddef>
#include <cstdlib>

FileTransmitter::FileTransmitter(QObject *parent)
    : QSctpServer{parent}, clients() {
    listen(QHostAddress::Any, m_port);
    qDebug() << "Listening @ " << serverAddress() << ":" << serverPort();
    qDebug() << "Server status is " << this->isListening() << "("
             << errorString() << ")";
}

void FileTransmitter::incomingConnection(qintptr socketDescriptor) {
    QSctpSocket *sock = new QSctpSocket(0);
    sock->setSocketDescriptor(socketDescriptor);
    qDebug() << "Got incoming connection from " << sock->peerName() << " ("
             << sock->peerAddress() << ")";
    Host host = Host(sock, QHostInfo::fromName(sock->peerName()));
    qDebug() << "SD is " << socketDescriptor;
    clients.append(Host(host));
    emit newHost(clients.size() - 1);
}

void FileTransmitter::sendFileAt(QString path, Host host) {
    QSctpSocket *socket = host.socket;

    QByteArray  datagramOut;
    QDataStream streamOut(&datagramOut, QIODevice::WriteOnly);
    streamOut.setVersion(QDataStream::Qt_5_9);

    QByteArray  datagramIn;
    QDataStream streamIn(&datagramIn, QIODevice::ReadOnly);
    streamIn.setVersion(QDataStream::Qt_5_9);

    qDebug() << "Sending file to " << socket->peerName() << "("
             << socket->peerAddress() << ")";

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    uint header = 0xAA;

    streamOut << header; // send size packet
    streamOut << file.size();
    streamOut << file.fileName();
    socket->writeDatagram(datagramOut);
    datagramOut.clear();

    qDebug() << "writing size (" << file.size() << "b)";
    socket->waitForBytesWritten();

    qDebug() << "reading ACK";
    socket->waitForReadyRead(); // read ack packet

    qDebug() << "reading header";
    datagramIn = socket->readDatagram().data();
    streamIn >> header;

    if (header != 0xBA) {
        qDebug() << "Header is shit" << QString::number(header, 16);
        // qDebug() << dataStream;
        return;
    }
    qDebug() << "header OK";

    do {
        datagramOut.resize(0);
        datagramOut.fill('\0');
        streamOut.device()->reset();
        
        datagramIn.resize(0);
        datagramIn.fill('\0');
        streamIn.device()->reset();
        

        char  *ptr = (char *)malloc(DATAGRAM_SIZE);
        qint64 len = file.read(ptr, DATAGRAM_SIZE);
        qDebug() << "len" << len;

        qDebug() << "writing file header";
        streamOut << ((len == DATAGRAM_SIZE) ? 0xAB : 0xAC);

        streamOut.writeBytes(ptr, len);

        qDebug() << "writing file data (" << datagramOut.size() << "b)";
        socket->writeDatagram(datagramOut);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        qDebug() << "reading " << socket->bytesAvailable() << "bytes";
        while (socket->bytesAvailable() < 8)
            ;

        datagramIn = socket->readDatagram().data();
        streamIn >> header;

        if (header != 0xBB) {
            qDebug() << "Header is shit" << QString::number(header, 16)
                     << "Continuing anyway";
            // return;
        }
        qDebug() << "header OK";
    } while (!file.atEnd());
    qDebug() << "done";

    streamOut << 0xAC;
    socket->writeDatagram(datagramOut);
    datagramOut.clear();

    socket->waitForReadyRead(10000);
    datagramIn = socket->readDatagram().data();

    streamIn >> header;
};