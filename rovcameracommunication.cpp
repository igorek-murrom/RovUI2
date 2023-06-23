#include "rovcameracommunication.h"

RovCameraCommunication::RovCameraCommunication(QObject *parent)
    : QObject{parent}, socket(new Client(QUrl("ws://raspberrypi.local:8080"))) {
    connect(socket, SIGNAL(echoReady(QJsonObject)), this,
            SLOT(parse(QJsonObject)));
}

void RovCameraCommunication::sendPacket() {
    QJsonObject jsonObject;
    for (Setting setting : cameraSettings) {
        if (setting.currentValue < setting.maxValue and
            setting.currentValue > setting.minValue)
            jsonObject.insert(setting.name, setting.currentValue);
    }
    QJsonDocument doc(jsonObject);
    QString       strJson(doc.toJson(QJsonDocument::Compact));
    QString       message = QString::fromUtf8(strJson.toUtf8().constData());
    socket->sendText(message);
}

void RovCameraCommunication::echo() {
    QJsonObject jsonObject;
    jsonObject.insert("_type", "v4l2_ctrls/report");
    QJsonDocument doc(jsonObject);
    QString       strJson(doc.toJson(QJsonDocument::Compact));
    QString       message = QString::fromUtf8(strJson.toUtf8().constData());
    socket->sendText(message);
}

void RovCameraCommunication::parse(QJsonObject settings) {
    QJsonObject objectCamera = settings["camera"].toObject();
    settings                 = objectCamera["controls"].toObject();
    QStringList keys         = settings.keys();
    for (QString name : keys) {
        QJsonObject setting = settings[name].toObject();

        cameraSettings[name].minValue     = setting["value_min"].toInt();
        cameraSettings[name].maxValue     = setting["value_max"].toInt();
        cameraSettings[name].currentValue = setting["value_current"].toInt();
        cameraSettings[name].name         = name;
        QJsonObject menuSettings          = setting["menu"].toObject();
        QStringList menuKeys              = menuSettings.keys();
        for (auto menuName : menuKeys)
            cameraSettings[name].menu[menuName] =
                menuSettings[menuName].toInt();
        qDebug() << "cock name " << name << "\n MinVal " << cameraSettings[name].minValue << "\n MaxVal " << cameraSettings[name].maxValue << "\n CurVal " << cameraSettings[name].currentValue;
    }
}
