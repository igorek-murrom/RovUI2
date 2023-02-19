#ifndef JOYSTICKSETUPDIALOG_H
#define JOYSTICKSETUPDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSettings>
#include "joystick.h"

namespace Ui {
class JoystickSetupDialog;
}

class JoystickSetupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit JoystickSetupDialog(QWidget *parent = nullptr);
    ~JoystickSetupDialog();
signals:
    void settingsUpdated();

public slots:
    void updateUi(Joystick);
    void populateUi(Joystick);

private slots:
    void updateAxesSettings(int);

private:
    void createConnections();

    Ui::JoystickSetupDialog *ui;

    QScopedPointer<QSettings> m_settings;

    QString m_axesNames[6] = { "AxisX", "AxisY", "AxisZ",
                               "AxisW", "AxisD", "AxisR" };
    QString m_buttonsNames[16] = {
                                 "ManipOpen", "ManipClose", "CamSel", "TSF-F",
                                 "TSF-M", "TSF-S", "Button7", "Button8",
                                 "Button9", "Button10", "Button11", "Button12",
                                 "Button13", "Button14", "Button15", "Button16"};
    QString m_hatsNames[4] = { "CamServo", "Hat2", "Hat3", "Hat4"};
    QString m_joystick_name;
};

#endif // JOYSTICKSETUPDIALOG_H
