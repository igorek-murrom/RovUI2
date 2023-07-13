#include "joysticksetupdialog.h"
#include "ui_joysticksetupdialog.h"

JoystickSetupDialog::JoystickSetupDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::JoystickSetupDialog),
      m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope,
                               "CfRD", "RovUI2", this))

{
    ui->setupUi(this);
    m_axesComboBoxes.append(ui->xAxisComboBox);
    m_axesComboBoxes.append(ui->yAxisComboBox);
    m_axesComboBoxes.append(ui->zAxisComboBox);
    m_axesComboBoxes.append(ui->wAxisComboBox);
    m_axesComboBoxes.append(ui->dAxisComboBox);
    m_axesComboBoxes.append(ui->rAxisComboBox);

    m_axesProgressBars.append(ui->xAxisProgressBar);
    m_axesProgressBars.append(ui->yAxisProgressBar);
    m_axesProgressBars.append(ui->zAxisProgressBar);
    m_axesProgressBars.append(ui->wAxisProgressBar);
    m_axesProgressBars.append(ui->dAxisProgressBar);
    m_axesProgressBars.append(ui->rAxisProgressBar);

    // welp ._.
    m_buttonsComboBoxes.append(ui->button1CB);
    m_buttonsComboBoxes.append(ui->button2CB);
    m_buttonsComboBoxes.append(ui->button3CB);
    m_buttonsComboBoxes.append(ui->button4CB);
    m_buttonsComboBoxes.append(ui->button5CB);
    m_buttonsComboBoxes.append(ui->button6CB);
    m_buttonsComboBoxes.append(ui->button7CB);
    m_buttonsComboBoxes.append(ui->button8CB);
    m_buttonsComboBoxes.append(ui->button9CB);
    m_buttonsComboBoxes.append(ui->button10CB);
    m_buttonsComboBoxes.append(ui->button11CB);
    m_buttonsComboBoxes.append(ui->button12CB);
    m_buttonsComboBoxes.append(ui->button13CB);
    m_buttonsComboBoxes.append(ui->button14CB);
    m_buttonsComboBoxes.append(ui->button15CB);
    m_buttonsComboBoxes.append(ui->button16CB);

    m_buttonsCheckBoxes.append(ui->button1ID);
    m_buttonsCheckBoxes.append(ui->button2ID);
    m_buttonsCheckBoxes.append(ui->button3ID);
    m_buttonsCheckBoxes.append(ui->button4ID);
    m_buttonsCheckBoxes.append(ui->button5ID);
    m_buttonsCheckBoxes.append(ui->button6ID);
    m_buttonsCheckBoxes.append(ui->button7ID);
    m_buttonsCheckBoxes.append(ui->button8ID);
    m_buttonsCheckBoxes.append(ui->button9ID);
    m_buttonsCheckBoxes.append(ui->button10ID);
    m_buttonsCheckBoxes.append(ui->button11ID);
    m_buttonsCheckBoxes.append(ui->button12ID);
    m_buttonsCheckBoxes.append(ui->button13ID);
    m_buttonsCheckBoxes.append(ui->button14ID);
    m_buttonsCheckBoxes.append(ui->button15ID);
    m_buttonsCheckBoxes.append(ui->button16ID);

    m_hatsComboBoxes.append(ui->hat1ComboBox);
    m_hatsComboBoxes.append(ui->hat2ComboBox);
    m_hatsComboBoxes.append(ui->hat3ComboBox);
    m_hatsComboBoxes.append(ui->hat4ComboBox);

    m_hatsCheckBoxes.append(ui->hat1CheckBox);
    m_hatsCheckBoxes.append(ui->hat2CheckBox);
    m_hatsCheckBoxes.append(ui->hat3CheckBox);
    m_hatsCheckBoxes.append(ui->hat4CheckBox);

    m_hatsSliders.append(ui->hat1PB);
    m_hatsSliders.append(ui->hat2PB);
    m_hatsSliders.append(ui->hat3PB);
    m_hatsSliders.append(ui->hat4PB);

    setWindowTitle(QString("Joystick settings - No Joystick"));
    populateUi(Joystick());

    createConnections();
}

JoystickSetupDialog::~JoystickSetupDialog() { delete ui; }

void JoystickSetupDialog::createConnections() {
    for (QComboBox *cb : qAsConst(m_axesComboBoxes)) {
        connect(cb, SIGNAL(currentIndexChanged(int)), this,
                SLOT(writeAxesSettings(int)));
    }
    for (QComboBox *cb : qAsConst(m_buttonsComboBoxes)) {
        connect(cb, SIGNAL(currentIndexChanged(int)), this,
                SLOT(writeButtonsSettings(int)));
    }
    for (QComboBox *cb : qAsConst(m_hatsComboBoxes)) {
        connect(cb, SIGNAL(currentIndexChanged(int)), this,
                SLOT(writeHatsSettings(int)));
    }
    for (QCheckBox *cb : qAsConst(m_hatsCheckBoxes)) {
        connect(cb, SIGNAL(stateChanged(int)), this,
                SLOT(writeHatsSettings(int)));
    }
}

void JoystickSetupDialog::writeAxesSettings(int) {
    if (m_ignoreUpdateRequest)
        return;

    for (uint i = 0; QComboBox * cb : qAsConst(m_axesComboBoxes)) {
        m_settings->setValue(m_joystick_name + QString("/joystickAxes/") +
                                 JoystickHelpers::axesNames[i],
                             cb->currentIndex() - 1);
        i++;
    }
    m_settings->sync();
    qInfo() << "Axes settings updated";
    emit settingsUpdated();
}

void JoystickSetupDialog::writeButtonsSettings(int) {
    if (m_ignoreUpdateRequest)
        return;

    for (uint i = 0; QComboBox * cb : qAsConst(m_buttonsComboBoxes)) {
        m_settings->setValue(m_joystick_name + QString("/joystickButtons/") +
                                 JoystickHelpers::buttonsNames[i],
                             cb->currentIndex() - 1);
        i++;
    }

    m_settings->sync();
    qInfo() << "Buttons settings updated";
    emit settingsUpdated();
}

void JoystickSetupDialog::writeHatsSettings(int) {
    if (m_ignoreUpdateRequest)
        return;

    for (uint i = 0; QComboBox * cb : qAsConst(m_hatsComboBoxes)) {
        m_settings->setValue(m_joystick_name + QString("/joystickHats/") +
                                 JoystickHelpers::hatsNames[i],
                             cb->currentIndex() - 1);
        i++;
    }
    for (uint i = 0; QCheckBox * cb : qAsConst(m_hatsCheckBoxes)) {
        m_settings->setValue(m_joystick_name + QString("/joystickHatsHor/") +
                                 JoystickHelpers::hatsNames[i],
                             cb->isChecked());
        i++;
    }
    m_settings->sync();
    qInfo() << "Hats settings updated";
    emit settingsUpdated();
}

void JoystickSetupDialog::showEvent(QShowEvent *) {}

void JoystickSetupDialog::populateUi(Joystick joy) {
    m_joystick_name =
        ((joy.joystickName == nullptr || joy.joystickName.isEmpty())
             ? QString("No Joystick Found")
             : joy.joystickName);
    setWindowTitle(QString("Joystick settings - ") + m_joystick_name);
    m_ignoreUpdateRequest = true;
    for (uint i = 0; QComboBox * cb : qAsConst(m_axesComboBoxes)) {
        cb->clear();
        cb->addItem(QString("Unset"));
        for (int j = 1; j <= joy.numAxes; j++) {
            cb->addItem(QString("Axis ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings
                                ->value(m_joystick_name +
                                        QString("/joystickAxes/") +
                                        JoystickHelpers::axesNames[i])
                                .toInt() +
                            1);
        i++;
    }

    for (uint i = 0; QComboBox * cb : qAsConst(m_buttonsComboBoxes)) {
        cb->clear();
        cb->addItem(QString("Unset"));
        for (int j = 1; j <= joy.numButtons; j++) {
            cb->addItem(QString("Button ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings
                                ->value(m_joystick_name +
                                        QString("/joystickButtons/") +
                                        JoystickHelpers::buttonsNames[i])
                                .toInt() +
                            1);
        i++;
    }
    for (uint i = 0; QComboBox * cb : qAsConst(m_hatsComboBoxes)) {
        cb->clear();
        cb->addItem(QString("Unset"));
        for (int j = 1; j <= joy.numHats; j++) {
            cb->addItem(QString("Hat ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings
                                ->value(m_joystick_name +
                                        QString("/joystickHats/") +
                                        JoystickHelpers::hatsNames[i])
                                .toInt() +
                            1);
        i++;
    }
    for (uint i = 0; QCheckBox * cb : qAsConst(m_hatsCheckBoxes)) {
        cb->setCheckState(m_settings
                                  ->value(QString(m_joystick_name) +
                                          QString("/joystickHatsHor/") +
                                          JoystickHelpers::hatsNames[i])
                                  .toBool()
                              ? Qt::CheckState::PartiallyChecked
                              : Qt::CheckState::Unchecked);
    }

    m_ignoreUpdateRequest = false;
}

void JoystickSetupDialog::updateUi(Joystick joystick) {
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        for (uint i = 0; QProgressBar * pb : qAsConst(m_axesProgressBars)) {
            pb->setValue(joystick.axes[i]);
            i++;
        }
        break;
    case 1:
        for (uint i = 0; QCheckBox * chb : qAsConst(m_buttonsCheckBoxes)) {
            chb->setCheckState(BIT_CHECK(joystick.buttons.rawData, i)
                                   ? Qt::CheckState::PartiallyChecked
                                   : Qt::CheckState::Unchecked);
            i++;
        }
        break;
    case 2:
        for (uint i = 0; QProgressBar * pb : qAsConst(m_hatsSliders)) {
            pb->setValue(joystick.hats[i]);
            i++;
        }
        break;
    default:
        break;
    }
}
