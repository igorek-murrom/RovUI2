#ifndef FILETRANSMITTER_H
#define FILETRANSMITTER_H

#include "qglobal.h"
#include "qhostaddress.h"
#include "qhostinfo.h"
#include "qlist.h"
#include "qobjectdefs.h"
#include "qscopedpointer.h"
#include "qsctpserver.h"
#include "qsctpsocket.h"
#include "qtcpserver.h"
#include "qtcpsocket.h"
#include "qthread.h"
#include "QFile"
#include "QDataStream"
#include <QObject>
#include <cstddef>

#define DATAGRAM_SIZE 65000

class FileTransmitter : public QSctpServer {
    Q_OBJECT

    struct Host {
        Host(QSctpSocket *socket, QHostInfo hostInfo)
            : socket(socket),
              hostInfo(hostInfo) {}
        QSctpSocket *socket;
        QHostInfo hostInfo;
    };

  public:
    explicit FileTransmitter(QObject *parent = nullptr);
    QList<Host> clients;

  signals:
    void newHost(qint64 index);

  public slots:

    void sendFileAt(QString path, Host host);

  private slots:

    void incomingConnection(qintptr socketDescriptor) override;

  private:
    const static int m_port = 46276;

    
};

#endif // FILETRANSMITTER_H
