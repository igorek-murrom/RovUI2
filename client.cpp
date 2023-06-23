#include "client.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

Client::Client(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Client::closedConnect);
    m_webSocket.open(QUrl(url));
}

void Client::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &Client::onTextMessageReceived);
}

void Client::onTextMessageReceived(QString message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject object = doc.object();
    if (object["_type"] == "v4l2_ctrls/report") {
        emit echoReady(object);
    }
    qDebug() << message;
}

void Client::closedConnect() {
    m_webSocket.close();
}

void Client::sendText(QString message) {
    m_webSocket.sendTextMessage(message);
}
