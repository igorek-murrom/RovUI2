#ifndef FILETRANSMITTER_H
#define FILETRANSMITTER_H

#include "QDataStream"
#include "QFile"
#include "qhostinfo.h"
#include "qobjectdefs.h"
#include "qscopedpointer.h"
#include "qthread.h"
#include "qtcpsocket.h"
#include "qtcpserver.h"
#include <QObject>
#include <cstddef>

#define DATAGRAM_SIZE 131000
#define SERVER_PORT 46276

struct Host {
    Host() : socket(new QTcpSocket()), hostInfo(){};
    Host(QTcpSocket *socket, QHostInfo hostInfo)
        : socket(socket), hostInfo(hostInfo) {}
    QTcpSocket *socket;
    QHostInfo   hostInfo;
};

class FileWorkerThread : public QThread {
    Q_OBJECT
  public:
    enum Headers {
        HeaderBeginOut = 0xAA,
        HeaderFileOut  = 0xAB,
        HeaderEndOut   = 0xAC,

        HeaderRBeginIn = 0xBA,
        HeaderRFileIn  = 0xBB,
        HeaderREndIn   = 0xBC
    };

    uint8_t progress;

  public slots:
    void setPath(QString path) { pendingPath = path; }
    void setHost(Host host) { pendingHost = host; }
    void run() override;


  private:
    QString pendingPath;
    Host    pendingHost;
};

class FileTransmitter : public QTcpServer {
    Q_OBJECT

  public:
    explicit FileTransmitter(QObject *parent = nullptr);

    QList<Host> clients;

  signals:
    void newHost(qint64 index);
    void removeHost(qint64 index);

  public slots:
    void setPath(QString path) { m_networkThread->setPath(path); };
    void setHost(Host host) { m_networkThread->setHost(host); };
    void sendFile() { m_networkThread->start(); };

  private slots:

    void incomingConnection(qintptr socketDescriptor) override;

  private:
    const static int m_port = 46276;

    QScopedPointer<FileWorkerThread> m_networkThread;
};

#endif // FILETRANSMITTER_H
