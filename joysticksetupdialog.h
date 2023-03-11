#ifndef JOYSTICKSETUPDIALOG_H
#define JOYSTICKSETUPDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSettings>
#include <QComboBox>
#include <QCheckBox>
#include <QProgressBar>
#include "joystick.h"
#include "helpers.h"

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
    virtual void showEvent(QShowEvent *event) override;
    void createConnections();

    Ui::JoystickSetupDialog *ui;

    QScopedPointer<QSettings> m_settings;
    QScopedPointer<Joystick> m_joystick;

    QList<QComboBox *> m_axesComboBoxes;
    QList<QProgressBar *> m_axesProgressBars;

    QList<QComboBox *> m_buttonsComboBoxes;
    QList<QCheckBox *> m_buttonsCheckBoxes;

    QList<QComboBox *> m_hatsComboBoxes;
    QList<QCheckBox *> m_hatsCheckBoxes;
    QList<QProgressBar *> m_hatsSliders;

    QString m_joystick_name;

    bool m_ignoreUpdateRequest = false;
};

#endif // JOYSTICKSETUPDIALOG_H
