#include "rovcameracommunication.h"

RovCameraCommunication::RovCameraCommunication(QObject *parent)
    : QObject{parent}, socket(new Client(QUrl("ws://localhost:8080"))) {
    connect(socket, SIGNAL(recieveReady(QJsonObject)), this, SLOT(processingMessage(QJsonObject)));
    connect(this, SIGNAL(reportReady(QJsonObject)), this, SLOT(parseSettings(QJsonObject)));
    connect(this, SIGNAL(outputReady(QJsonObject)), this, SLOT(displayOutput(QJsonObject)));
}

void RovCameraCommunication::sendJSON(QJsonObject jsonObject) {
    QJsonDocument doc(jsonObject);
    QString       strJson(doc.toJson(QJsonDocument::Compact));
    QString       message = QString::fromUtf8(strJson.toUtf8().constData());
    socket->sendText(message);
}

void RovCameraCommunication::processingMessage(QJsonObject jsonObject) {
    if (jsonObject["_type"] == "v4l2_ctrls/report") {
        emit reportReady(jsonObject);
    }
    if (jsonObject["_type"] == "gst_pipes/output") {
        emit outputReady(jsonObject);
    }
}

void RovCameraCommunication::sendPacket() {
    QJsonObject jsonObject;
    for (Setting setting : cameraSettings) {
        if (setting.currentValue < setting.maxValue and
            setting.currentValue > setting.minValue)
            jsonObject.insert(setting.name, setting.currentValue);
    }
    sendJSON(jsonObject);
}

void RovCameraCommunication::echo() {
    QJsonObject jsonObject;
    jsonObject.insert("_type", "v4l2_ctrls/report");
    sendJSON(jsonObject);
}

void RovCameraCommunication::startStream() {
    QJsonObject jsonObject;
    jsonObject.insert("_type", "gst_pipes/start");
    sendJSON(jsonObject);
}

void RovCameraCommunication::stopStream() {
    QJsonObject jsonObject;
    jsonObject.insert("_type", "gst_pipes/stop");
    sendJSON(jsonObject);
}

void RovCameraCommunication::parseSettings(QJsonObject settings) {
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
        qDebug() << "Name " << name << "\n MinVal " << cameraSettings[name].minValue << "\n MaxVal " << cameraSettings[name].maxValue << "\n CurVal " << cameraSettings[name].currentValue;
    }
    emit cameraSettingsReady(cameraSettings);
}

void RovCameraCommunication::displayOutput(QJsonObject message) {
    QByteArray ba; ba.append(message["data"].toString());
    qDebug() << "(type: " << message["log_type"].toString() << ") " << QByteArray::fromBase64(ba);
}
