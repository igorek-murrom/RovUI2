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
    m_axesComboBoxes[3] = ui->dAxisComboBox;
    m_axesComboBoxes[4] = ui->wAxisComboBox;
    m_axesComboBoxes[5] = ui->rAxisComboBox;

    m_axesProgressBars[0] = ui->xAxisProgressBar;
    m_axesProgressBars[1] = ui->yAxisProgressBar;
    m_axesProgressBars[2] = ui->zAxisProgressBar;
    m_axesProgressBars[3] = ui->wAxisProgressBar;
    m_axesProgressBars[4] = ui->dAxisProgressBar;
    m_axesProgressBars[5] = ui->rAxisProgressBar;

    //welp
    m_buttonsComboBoxes[0] = ui->openManipCB;
    m_buttonsComboBoxes[1] = ui->closeManipCB;
    m_buttonsComboBoxes[2] = ui->camSelCB;
    m_buttonsComboBoxes[3] = ui->asfFCB;
    m_buttonsComboBoxes[4] = ui->asfMCB;
    m_buttonsComboBoxes[5] = ui->asfSCB;
    m_buttonsComboBoxes[6] = ui->asfUSCB;
    m_buttonsComboBoxes[7] = ui->button8CB;
    m_buttonsComboBoxes[8] = ui->button9CB;
    m_buttonsComboBoxes[9] = ui->button10CB;
    m_buttonsComboBoxes[10] = ui->button11CB;
    m_buttonsComboBoxes[11] = ui->button12CB;
    m_buttonsComboBoxes[12] = ui->button13CB;
    m_buttonsComboBoxes[13] = ui->button14CB;
    m_buttonsComboBoxes[14] = ui->button15CB;
    m_buttonsComboBoxes[15] = ui->button16CB;

    m_buttonsCheckBoxes[0] = ui->openManipID;
    m_buttonsCheckBoxes[1] = ui->closeManipID;
    m_buttonsCheckBoxes[2] = ui->camSelID;
    m_buttonsCheckBoxes[3] = ui->asfFID;
    m_buttonsCheckBoxes[4] = ui->asfMID;
    m_buttonsCheckBoxes[5] = ui->asfSID;
    m_buttonsCheckBoxes[6] = ui->asfUSID;
    m_buttonsCheckBoxes[7] = ui->button8ID;
    m_buttonsCheckBoxes[8] = ui->button9ID;
    m_buttonsCheckBoxes[9] = ui->button10ID;
    m_buttonsCheckBoxes[10] = ui->button11ID;
    m_buttonsCheckBoxes[11] = ui->button12ID;
    m_buttonsCheckBoxes[12] = ui->button13ID;
    m_buttonsCheckBoxes[13] = ui->button14ID;
    m_buttonsCheckBoxes[14] = ui->button15ID;
    m_buttonsCheckBoxes[15] = ui->button16ID;

    m_hatsComboBoxes[0] = ui->hatCamServoCB;
    m_hatsComboBoxes[1] = ui->hat2CB;
    m_hatsComboBoxes[2] = ui->hat3CB;
    m_hatsComboBoxes[3] = ui->hat4CB;

    m_hatsSliders[0] = ui->hatCamServoPB;
    m_hatsSliders[1] = ui->hat2PB;
    m_hatsSliders[2] = ui->hat3PB;
    m_hatsSliders[3] = ui->hat4PB;

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
        m_settings->setValue(m_joystick_name + QString("/joystickAxes/") + JoystickNames::axesNames[i], cb->currentIndex());
        i++;
    }
    m_settings->sync();
    qInfo() << "Axes settings updated" << Qt::endl;
    emit settingsUpdated();
}

void JoystickSetupDialog::updateButtonsSettings(int){
    if(m_ignoreUpdateRequest) return;

    for (uint i = 0; QComboBox *cb : m_buttonsComboBoxes) {
        m_settings->setValue(m_joystick_name + QString("/joystickButtons/") + JoystickNames::buttonsNames[i], cb->currentIndex());
        i++;
    }

    m_settings->sync();
    qInfo() << "Buttons settings updated" << Qt::endl;
    emit settingsUpdated();
}

void JoystickSetupDialog::updateHatsSettings(int){
    if(m_ignoreUpdateRequest) return;

    for (uint i = 0; QComboBox *cb : m_buttonsComboBoxes) {
        m_settings->setValue(m_joystick_name + QString("/joystickHats/") + JoystickNames::buttonsNames[i], cb->currentIndex());
        i++;
    }

    m_settings->sync();
    qInfo() << "Hats settings updated" << Qt::endl;
    emit settingsUpdated();
}


void JoystickSetupDialog::populateUi(Joystick joy){
    m_joystick_name = joy.joystickName;
    setWindowTitle(QString("Joystick settings - ") + joy.joystickName);
    m_ignoreUpdateRequest = true;
    for (uint i = 0; QComboBox *cb : m_axesComboBoxes) {
        cb->clear();
        for(int j = 1; j <= joy.numAxes; j++){
            cb->addItem(QString("Axis ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickAxes/") + JoystickNames::axesNames[i]).toInt());
        i++;
    }

    for (uint i = 0; QComboBox *cb : m_buttonsComboBoxes) {
        cb->clear();
        for(int j = 1; j <= joy.numButtons; j++){
            cb->addItem(QString("Button ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickButtons/") + JoystickNames::buttonsNames[i]).toInt());
        i++;
    }
    for (uint i = 0; QComboBox *cb : m_hatsComboBoxes) {
        cb->clear();
        for(int j = 1; j <= joy.numButtons; j++){
            cb->addItem(QString("Hat ") + QString::number(j));
        }
        cb->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickHats/") + JoystickNames::hatsNames[i]).toInt());
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
            chb->setCheckState(joystick.buttons ? Qt::CheckState::PartiallyChecked : Qt::CheckState::Unchecked);
        }
    default:
        break;
    }
    for (int i = 0; i < 6; i++) {
        m_axesProgressBars[i]->setValue(joystick.axes[i]);
    }
}
