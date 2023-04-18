#ifndef THRUSTERSETUPDIALOG_H
#define THRUSTERSETUPDIALOG_H

#include "qlabel.h"
#include <QDialog>
#include <QSettings>

namespace Ui {
    class ThrusterSetupDialog;
}

/*!
 * \brief The ThrusterSetupDialog class is responsible for the corresponding
 * dialog
 */
class ThrusterSetupDialog : public QDialog {
    Q_OBJECT

  public:
    /*!
     * \brief Default constructor
     * \param parent Parent
     */
    explicit ThrusterSetupDialog(QWidget *parent = nullptr);

    /*!
     * \brief Default destructor
     */
    ~ThrusterSetupDialog();

  private slots:
    /*!
     * \brief Packs sliders data and emits thrustersOverridden
     * \param index index of the slider
     * \param state position of the slider
     */
    void updateSliders(int index, int state);
    /*!
     * \brief Packs invert data and emits invertsOverridden
     * \param index index of the invert button
     * \param state State of the button
     */
    void updateInvert(int index, int state);

    /*!
     * \brief Emits overrideStatusChanged(true) if state>0 else emits
     * overrideStatusChanged(false) \param state State
     */
    void changeOverrideStatus(int state);

  signals:
    /*!
     * \brief Emitted when overrides turn on and off
     * \param status Override status
     */
    void overrideStatusChanged(bool status);

    /*!
     * \brief Emitted when sliders' positions change
     */
    void thrustersOverridden(QList<qint8>);
    /*!
     * \brief Emitted when buttons' states change
     */
    void invertsOverridden(qint8);

  private:
    /*!
     * \brief Connects UI to the slots etc etc
     */
    void createConnections();

    /*!
     * \brief Settings object
     */
    QScopedPointer<QSettings> m_settings;

    /*!
     * \brief Value labels for sliders
     */
    QLabel *m_dataLabels[8];

    /*!
     * \brief UI object
     */
    Ui::ThrusterSetupDialog *ui;

    /*!
     * \brief QList of override values, ready to be sent whenever needed
     */
    QList<qint8> thrustersOverride = {0, 0, 0, 0, 0, 0, 0, 0};

    /*!
     * \brief Packed buttons' states
     */
    qint8 invertOverride;
};

#endif // THRUSTERSETUPDIALOG_H
