#ifndef ROVCAMERACOMMUNICATION_H
#define ROVCAMERACOMMUNICATION_H

#include <QObject>
#include <client.h>
#include <QDebug>
#include <QMap>
struct Setting {
    int maxValue;
    int minValue;
    int currentValue;
    QString name;
    QMap <QString, int> menu;

    //    bool hasChanged;
};

class RovCameraCommunication : public QObject
{
    Q_OBJECT
public:
    explicit RovCameraCommunication(QObject *parent = nullptr);

signals:
    void reportReady(QJsonObject jsonObject);
    void outputReady(QJsonObject jsonObject);
    void cameraSettingsReady(QMap<QString, Setting>);

public slots:
    void sendPacket();
    void echo();
    void startStream();
    void stopStream();

private slots:
    void processingMessage(QJsonObject jsonObject);
    void parseSettings(QJsonObject settings);
    void displayOutput(QJsonObject message);

private:
    Client *socket;
    QMap<QString, Setting> cameraSettings;
    void sendJSON(QJsonObject jsonObject);
};

#endif // ROVCAMERACOMMUNICATION_H
