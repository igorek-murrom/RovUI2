#ifndef FILETRANSMITTER_H
#define FILETRANSMITTER_H

#include "qglobal.h"
#include "qhostaddress.h"
#include "qhostinfo.h"
#include "qlist.h"
#include "qobjectdefs.h"
#include "qscopedpointer.h"
#include "qtcpserver.h"
#include "qtcpsocket.h"
#include "qthread.h"
#include "QFile"
#include "QDataStream"
#include <QObject>
#include <cstddef>

class FileTransmitter : public QTcpServer {
    Q_OBJECT

    struct Host {
        Host(QTcpSocket *socket, QHostInfo hostInfo)
            : socket(socket),
              hostInfo(hostInfo) {}
        QTcpSocket *socket;
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
