#ifndef JOYSTICKSETUPDIALOG_H
#define JOYSTICKSETUPDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSettings>
#include <QComboBox>
#include <QCheckBox>
#include <QProgressBar>
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
    void updateButtonsSettings(int);
    void updateHatsSettings(int);

private:
    void createConnections();

    Ui::JoystickSetupDialog *ui;

    QScopedPointer<QSettings> m_settings;

    QComboBox *m_axesComboBoxes[6];
    QProgressBar *m_axesProgressBars[6];

    QComboBox *m_buttonsComboBoxes[16];
    QCheckBox *m_buttonsCheckBoxes[16];

    QComboBox *m_hatsComboBoxes[4];
    QProgressBar *m_hatsSliders[4];

    QString m_joystick_name;

    bool m_ignoreUpdateRequest = false;
};

#endif // JOYSTICKSETUPDIALOG_H
