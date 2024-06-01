#ifndef JOYSTICKSETUPDIALOG_H
#define JOYSTICKSETUPDIALOG_H

#include "joystick.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QProgressBar>
#include <QSettings>

namespace Ui {
    class JoystickSetupDialog;
}

/**
 * \brief The JoystickSetupDialog class is responsible for the dialog which
 * allows user to setup their joystick
 */
class JoystickSetupDialog : public QDialog {
    Q_OBJECT
  public:
    /**
     * \brief Explicit default constructor
     * \param parent Parent
     */
    explicit JoystickSetupDialog(QWidget *parent = nullptr);
    /**
     * \brief Default destructor
     */
    ~JoystickSetupDialog();
  signals:
    /**
     * \brief Emitted when user changes settings or closes the dualog
     */
    void settingsUpdated();

  public slots:
    /**
     * \brief Called to update UI based on joystick data(like axes, buttons etc)
     * \param joy a Joystick struct with info about the joystick
     */
    void updateUi(Joystick joy);

    /**
     * \brief Called to redo the UI based on joystick data i.e. when user
     * (un)plugs joystick \param joy a Joystick struct with info about the
     * joystick
     */
    void populateUi(Joystick joy);

  private slots:

    /**
     * \brief Called to write axes settings to settings file
     */
    void writeAxesSettings(int);

    /**
     * \brief Called to write buttons settings to settings file
     */
    void writeButtonsSettings(int);

    /**
     * \brief Called to write hats settings to settings file
     */
    void writeHatsSettings(int);

  private:
    /**
     * \brief Unused override
     * \param event Event
     */
    virtual void showEvent(QShowEvent *event) override;
    void         createConnections();

    /**
     * \brief UI object
     */
    Ui::JoystickSetupDialog *ui;

    /**
     * \brief Settings object
     * \todo Make settings object into separate module??
     */
    QScopedPointer<QSettings> m_settings;

    /**
     * \brief Joystick object used to populate and update UI
     */
    QScopedPointer<Joystick> m_joystick;

    /**
     * \brief Helper QList with axes' QComboBoxes, used to select joystick axis
     * \todo migrate to QScopedPointer
     */
    QList<QComboBox *>    m_axesComboBoxes;
    /**
     * \brief Helper QList with axes' QProgressBars, used to display axis' value
     * \todo migrate to QScopedPointer
     */
    QList<QProgressBar *> m_axesProgressBars;

    /**
     * \brief Helper QList with buttons' QComboBoxes, used to select joystick
     * buttons \todo migrate to QScopedPointer
     */
    QList<QComboBox *> m_buttonsComboBoxes;
    /**
     * \brief Helper QList with buttons' QCheckBoxes, used to display button
     * status \todo migrate to QScopedPointer \todo make a proper indicator
     */
    QList<QCheckBox *> m_buttonsCheckBoxes;

    /**
     * \brief Helper QList with hats' QComboBoxes, used to select joystick hats
     * \todo migrate to QScopedPointer
     */
    QList<QComboBox *>    m_hatsComboBoxes;
    /**
     * \brief Helper QList with hats' QCheckBoxes, used to select joystick hats'
     * orientation \todo migrate to QScopedPointer \see Joystick::hats_hor
     */
    QList<QCheckBox *>    m_hatsCheckBoxes;
    /**
     * \brief Helper QList with hats' QProgressBars, used to display joystick
     * hats' state \todo migrate to QScopedPointer
     */
    QList<QProgressBar *> m_hatsSliders;

    /**
     * \brief Joystick name used in window title
     */
    QString m_joystick_name;

    /**
     * \brief Internal flag used to update values seamlessly
     */
    bool m_ignoreUpdateRequest = false;
};

#endif // JOYSTICKSETUPDIALOG_H
