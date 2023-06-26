#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QMainWindow>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QUrl &url, QObject *parent = nullptr);

signals:
    void reportReady(QJsonObject obj);
public slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void sendText(QString message);
    void closedConnect();

private:
    QWebSocket m_webSocket;
    QUrl m_url;
};

#endif // CLIENT_H
