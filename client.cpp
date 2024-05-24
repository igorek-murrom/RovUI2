#include "client.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

Client::Client(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_timerConnect(new QTimer(this))
{
    connect(&m_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Client::closedConnect);
    connect(&m_webSocket, &QWebSocket::stateChanged, this, &Client::stateCheck);
    connect(m_timerConnect, SIGNAL(timeout()), this, SLOT(tryConnect()));

    m_webSocket.open(m_url);
    m_timerConnect->setInterval(30);
}

void Client::onConnected()
{
    m_status = true;
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &Client::onTextMessageReceived);
}

void Client::closedConnect() {
    m_status = false;
    closedSocket();
}

void Client::tryConnect() {
    m_webSocket.close();
    m_webSocket.open(m_url);
}

void Client::stateCheck(QAbstractSocket::SocketState state) {
    if (state == QAbstractSocket::UnconnectedState) m_timerConnect->start();
    if (state == QAbstractSocket::ConnectedState) m_timerConnect->stop();
}

void Client::onTextMessageReceived(QString message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject object = doc.object();
    emit recieveReady(object);
}

void Client::sendText(QString message) {
    if(m_status) m_webSocket.sendTextMessage(message);
}

void Client::closedSocket() {
    m_webSocket.close();
}
