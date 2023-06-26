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
    void settingsReady();

  public slots:
    //    void Brightness(int value);
    //    void Contrast(int value);
    //    void Saturation(int value);
    //    void Hue(int value);
    //    void Gamma(int value);
    //    void Sharpness(int value);
    //    void Gain(int value);
    //    void White_Balance_Temperature_Auto(int value);
    //    void White_Balance_Temperature(int value);
    //    void Exposure_Auto(int value);
    //    void Exposure_Auto_Priority(int value);
    //    void Exposure_Absolute(int value);
    //    void Backlight_Compensation(int value);
    //    void Power_Line_Frequency(int value);

    //    void addSetting(Setting *setting);

    void sendPacket();
    void echo();
    void parse(QJsonObject objec);

  private:
    //    QList<Setting*> cameraSettings;
    Client                *socket;
    QMap<QString, Setting> cameraSettings;
};

#endif // ROVCAMERACOMMUNICATION_H
