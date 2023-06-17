#include "rovcameracommunication.h"

rovcameracommunication::rovcameracommunication(QObject *parent)
    : QObject{parent}
    , socket(new Client(QUrl("ws://192.168.0.161:8000")))
{
    connect(socket, SIGNAL(echoReady(QJsonObject)), this, SLOT(parsing(QJsonObject)));
}

void rovcameracommunication::sendPacket() {
    QJsonObject jsonObject;
    for(Setting setting : cameraSettings){
        if(setting.currentValue < setting.maxValue and setting.currentValue > setting.minValue)
            jsonObject.insert(setting.name, setting.currentValue);
    }
    QJsonDocument doc(jsonObject);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    QString message = QString::fromUtf8(strJson.toUtf8().constData());
    socket->sendText(message);
}

void rovcameracommunication::echo() {
    QJsonObject jsonObject;
    jsonObject.insert("_type", "v4l2_ctrls/report");
    QJsonDocument doc(jsonObject);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    QString message = QString::fromUtf8(strJson.toUtf8().constData());
    socket->sendText(message);
}

void rovcameracommunication::parsing(QJsonObject settings) {
    QJsonObject objectCamera = settings["camera"].toObject();
    settings = objectCamera["controls"].toObject();
    QStringList keys = settings.keys();
    for (QString name : keys) {
        QJsonObject set = settings[name].toObject();
        cameraSettings[name].minValue = set["value_min"].toInt();
        cameraSettings[name].maxValue = set["value_max"].toInt();
        cameraSettings[name].currentValue = set["value_current"].toInt();
        cameraSettings[name].name = name;
    }
}
