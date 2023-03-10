#include "joysticksetupdialog.h"
#include "ui_joysticksetupdialog.h"

JoystickSetupDialog::JoystickSetupDialog(QWidget *parent)
   : QDialog(parent)
   , ui(new Ui::JoystickSetupDialog)
   , m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "CfRD", "RovUI2", this))

{
    ui->setupUi(this);
    m_axesComboBoxes[0] = ui->xAxisComboBox;
    m_axesComboBoxes[1] = ui->yAxisComboBox;
    m_axesComboBoxes[2] = ui->zAxisComboBox;
    m_axesComboBoxes[3] = ui->wAxisComboBox;
    m_axesComboBoxes[4] = ui->dAxisComboBox;
    m_axesComboBoxes[5] = ui->rAxisComboBox;

    m_axesProgressBars[0] = ui->xAxisProgressBar;
    m_axesProgressBars[1] = ui->yAxisProgressBar;
    m_axesProgressBars[2] = ui->zAxisProgressBar;
    m_axesProgressBars[3] = ui->wAxisProgressBar;
    m_axesProgressBars[4] = ui->dAxisProgressBar;
    m_axesProgressBars[5] = ui->rAxisProgressBar;

    //welp ._.
    m_buttonsComboBoxes[0] = ui->button1CB;
    m_buttonsComboBoxes[1] = ui->button2CB;
    m_buttonsComboBoxes[2] = ui->button3CB;
    m_buttonsComboBoxes[3] = ui->button4CB;
    m_buttonsComboBoxes[4] = ui->button5CB;
    m_buttonsComboBoxes[5] = ui->button6CB;
    m_buttonsComboBoxes[6] = ui->button7CB;
    m_buttonsComboBoxes[7] = ui->button8CB;
    m_buttonsComboBoxes[8] = ui->button9CB;
    m_buttonsComboBoxes[9] = ui->button10CB;
    m_buttonsComboBoxes[10] = ui->button11CB;
    m_buttonsComboBoxes[11] = ui->button12CB;
    m_buttonsComboBoxes[12] = ui->button13CB;
    m_buttonsComboBoxes[13] = ui->button14CB;
    m_buttonsComboBoxes[14] = ui->button15CB;
    m_buttonsComboBoxes[15] = ui->button16CB;

    m_buttonsCheckBoxes[0] = ui->button1ID;
    m_buttonsCheckBoxes[1] = ui->button2ID;
    m_buttonsCheckBoxes[2] = ui->button3ID;
    m_buttonsCheckBoxes[3] = ui->button4ID;
    m_buttonsCheckBoxes[4] = ui->button5ID;
    m_buttonsCheckBoxes[5] = ui->button6ID;
    m_buttonsCheckBoxes[6] = ui->button7ID;
    m_buttonsCheckBoxes[7] = ui->button8ID;
    m_buttonsCheckBoxes[8] = ui->button9ID;
    m_buttonsCheckBoxes[9] = ui->button10ID;
    m_buttonsCheckBoxes[10] = ui->button11ID;
    m_buttonsCheckBoxes[11] = ui->button12ID;
    m_buttonsCheckBoxes[12] = ui->button13ID;
    m_buttonsCheckBoxes[13] = ui->button14ID;
    m_buttonsCheckBoxes[14] = ui->button15ID;
    m_buttonsCheckBoxes[15] = ui->button16ID;

    m_hatsComboBoxes[0] = ui->hat1ComboBox;
    m_hatsComboBoxes[1] = ui->hat2ComboBox;
    m_hatsComboBoxes[2] = ui->hat3ComboBox;
    m_hatsComboBoxes[3] = ui->hat4ComboBox;

    m_hatsCheckBoxes[0] = ui->hat1CheckBox;
    m_hatsCheckBoxes[1] = ui->hat2CheckBox;
    m_hatsCheckBoxes[2] = ui->hat3CheckBox;
    m_hatsCheckBoxes[3] = ui->hat4CheckBox;

    m_hatsSliders[0] = ui->hat1PB;
    m_hatsSliders[1] = ui->hat2PB;
    m_hatsSliders[2] = ui->hat3PB;
    m_hatsSliders[3] = ui->hat4PB;

    setWindowTitle(QString("Joystick settings - No Joystick"));
    populateUi(Joystick());

    createConnections();

}

JoystickSetupDialog::~JoystickSetupDialog()
{
    delete ui;
}

void JoystickSetupDialog::createConnections(){
    for (QComboBox *cb : m_axesComboBoxes) {
        connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    }
    for (QComboBox *cb : m_buttonsComboBoxes) {
        connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtonsSettings(int)));
    }
    for (QComboBox *cb : m_hatsComboBoxes) {
        connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateHatsSettings(int)));
    }
}

void JoystickSetupDialog::updateAxesSettings(int){
    if(m_ignoreUpdateRequest) return;

    for (uint i = 0; QComboBox *cb : m_axesComboBoxes) {
        m_settings->setValue(m_joystick_name + QString("/joystickAxes/") + JoystickNames::axesNames[i], cb->currentIndex() - 1);
        i++;
    }
    m_settings->sync();
    qInfo() << "Axes settings updated" << Qt::endl;
    emit settingsUpdated();
}

void JoystickSetupDialog::updateButtonsSettings(int){
    if(m_ignoreUpdateRequest) return;

    for (uint i = 0; QComboBox *cb : m_buttonsComboBoxes) {
        m_settings->setValue(m_joystick_name + QString("/joystickButtons/") + JoystickNames::buttonsNames[i], cb->currentIndex() - 1);
        i++;
    }

    m_settings->sync();
    qInfo() << "Buttons settings updated" << Qt::endl;
    emit settingsUpdated();
}

void JoystickSetupDialog::updateHatsSettings(int){
    if(m_ignoreUpdateRequest) return;

    for (uint i = 0; QComboBox *cb : m_hatsComboBoxes) {
        m_settings->setValue(m_joystick_name + QString("/joystickHats/") + JoystickNames::hatsNames[i], cb->currentIndex() - 1);
        i++;
    }
    for (uint i = 0; QCheckBox *cb : m_hatsCheckBoxes){
        m_settings->setValue(m_joystick_name + QString("/joystickHats/") + JoystickNames::hatsNames[i] + QString("Hor"), cb->isChecked());
        i++;
    }
    m_settings->sync();
    qInfo() << "Hats settings updated" << Qt::endl;
    emit settingsUpdated();
}


void JoystickSetupDialog::populateUi(Joystick joy){
    m_joystick_name = ((joy.joystickName == nullptr || joy.joystickName.isEmpty()) ? QString("No Joystick Found") : joy.joystickName);
    setWindowTitle(QString("Joystick settings - ") + ((joy.joystickName == nullptr || joy.joystickName.isEmpty()) ? QString("No Joystick Found") : joy.joystickName));
    m_ignoreUpdateRequest = true;
    for (uint i = 0; QComboBox *cb : m_axesComboBoxes) {
        cb->clear();
        cb->addItem(QString("Unset"));
        for(int j = 1; j <= joy.numAxes; j++){
            cb->addItem(QString("Axis ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickAxes/") + JoystickNames::axesNames[i]).toInt() + 1);
        i++;
    }

    for (uint i = 0; QComboBox *cb : m_buttonsComboBoxes) {
        cb->clear();
        cb->addItem(QString("Unset"));
        for(int j = 1; j <= joy.numButtons; j++){
            cb->addItem(QString("Button ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickButtons/") + JoystickNames::buttonsNames[i]).toInt() + 1);
        i++;
    }
    for (uint i = 0; QComboBox *cb : m_hatsComboBoxes) {
        cb->clear();
        cb->addItem(QString("Unset"));
        for(int j = 1; j <= joy.numHats; j++){
            cb->addItem(QString("Hat ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickHats/") + JoystickNames::hatsNames[i]).toInt() + 1);
        i++;
    }

    m_ignoreUpdateRequest = false;
}

void JoystickSetupDialog::updateUi(Joystick joystick){
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        for (uint i = 0; QProgressBar *pb : m_axesProgressBars) {
            pb->setValue(joystick.axes[i]);
            i++;
        }
        break;
    case 1:
        for (uint i = 0; QCheckBox *chb : m_buttonsCheckBoxes) {
            chb->setCheckState(BIT_CHECK(joystick.buttons, i) ? Qt::CheckState::PartiallyChecked : Qt::CheckState::Unchecked);
            i++;
        }
    case 2:
        for(uint i = 0; QProgressBar *pb : m_hatsSliders){
            pb->setValue(joystick.hats[i]);
            i++;
        }
    default:
        break;
    }
}
