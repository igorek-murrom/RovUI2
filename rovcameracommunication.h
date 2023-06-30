#ifndef ROVCAMERACOMMUNICATION_H
#define ROVCAMERACOMMUNICATION_H

#include "qglobal.h"
#include "qpair.h"
#include <QDebug>
#include <QMap>
#include <QObject>
#include <client.h>
struct Setting {
    int                maxValue;
    int                minValue;
    int                currentValue;
    QString            name;
    QMap<QString, int> menu;
    QString            toString() {
        QString ret_thingamabob;
        ret_thingamabob += name + "\n";
        if (!menu.isEmpty()) {
            ret_thingamabob += "is a menu entry\n";
            for (QString k : menu.keys()) {
                ret_thingamabob += k + ": " + QString::number(menu.value(k)) + "\n";
            }
            return ret_thingamabob;
        } else {
            ret_thingamabob += "is a normal entry\n";
            ret_thingamabob += QString("max value is: ") + QString::number(maxValue) + "\n";
            ret_thingamabob += QString("min value is: ") + QString::number(minValue) + "\n";
            ret_thingamabob += QString("cur value is: ") + QString::number(currentValue) + "\n";
            return ret_thingamabob;
        }
    };
    //    bool hasChanged;
};

class RovCameraCommunication : public QObject {
    Q_OBJECT
  public:
    explicit RovCameraCommunication(QObject *parent = nullptr);
    QMap<QString, Setting> getSettings();

signals:
    void reportReady(QJsonObject jsonObject);
    void outputReady(QJsonObject jsonObject);
    void cameraSettingsReady(QMap<QString, Setting>);

public slots:
    void echo();
    void startStream();
    void stopStream();
    void sendSettings(QMap<QString, Setting> settingsMap);

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
