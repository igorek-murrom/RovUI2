#ifndef ROVCAMERACOMMUNICATION_H
#define ROVCAMERACOMMUNICATION_H

#include <QObject>
#include <client.h>
#include <QDebug>
#include <QMap>
struct Setting {
    QString name;
    int maxValue;
    int minValue;
    int currentValue;
    //    bool hasChanged;
};

class rovcameracommunication : public QObject
{
    Q_OBJECT
public:
    explicit rovcameracommunication(QObject *parent = nullptr);

signals:

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
    void parsing(QJsonObject objec);

private:
//    QList<Setting*> cameraSettings;
    Client *socket;
    QMap<QString, Setting> cameraSettings;
};

#endif // ROVCAMERACOMMUNICATION_H
