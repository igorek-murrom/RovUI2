#include "joysticksetupdialog.h"
#include "ui_joysticksetupdialog.h"

JoystickSetupDialog::JoystickSetupDialog(QWidget *parent)
   : QDialog(parent)
   , ui(new Ui::JoystickSetupDialog)
   , m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "CfRD", "RovUI2", this))

{
    ui->setupUi(this);
    createConnections();
}

JoystickSetupDialog::~JoystickSetupDialog()
{
    delete ui;
}

void JoystickSetupDialog::createConnections(){
    connect(ui->xAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->yAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->zAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->wAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->dAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->rAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
}

void JoystickSetupDialog::updateAxesSettings(int){
    m_settings->setValue(m_joystick_name + QString("/joystickAxes/") + m_axesNames[0], ui->xAxisComboBox->currentIndex());
    m_settings->setValue(m_joystick_name + QString("/joystickAxes/") + m_axesNames[1], ui->yAxisComboBox->currentIndex());
    m_settings->setValue(m_joystick_name + QString("/joystickAxes/") + m_axesNames[2], ui->zAxisComboBox->currentIndex());
    m_settings->setValue(m_joystick_name + QString("/joystickAxes/") + m_axesNames[3], ui->wAxisComboBox->currentIndex());
    m_settings->setValue(m_joystick_name + QString("/joystickAxes/") + m_axesNames[4], ui->dAxisComboBox->currentIndex());
    m_settings->setValue(m_joystick_name + QString("/joystickAxes/") + m_axesNames[5], ui->rAxisComboBox->currentIndex());
    m_settings->sync();
    qInfo() << "settings updated" << Qt::endl;
    emit settingsUpdated();
}


void JoystickSetupDialog::populateUi(Joystick joy){
    m_joystick_name = *joy.joystickName;
    setWindowTitle(QString("Joystick settings - ") + *joy.joystickName);
    disconnect(ui->xAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    disconnect(ui->yAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    disconnect(ui->zAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    disconnect(ui->wAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    disconnect(ui->dAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    disconnect(ui->rAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));    ui->xAxisComboBox->clear();
    ui->yAxisComboBox->clear();
    ui->zAxisComboBox->clear();
    ui->wAxisComboBox->clear();
    ui->dAxisComboBox->clear();
    ui->rAxisComboBox->clear();
    for(int i = 1; i <= joy.numAxes; i++){
        ui->xAxisComboBox->addItem(QString("Axis ") + QString::number(i));
        ui->yAxisComboBox->addItem(QString("Axis ") + QString::number(i));
        ui->zAxisComboBox->addItem(QString("Axis ") + QString::number(i));
        ui->wAxisComboBox->addItem(QString("Axis ") + QString::number(i));
        ui->dAxisComboBox->addItem(QString("Axis ") + QString::number(i));
        ui->rAxisComboBox->addItem(QString("Axis ") + QString::number(i));
    }
    ui->xAxisComboBox->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickAxes/") + m_axesNames[0]).toInt());
    ui->yAxisComboBox->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickAxes/") + m_axesNames[1]).toInt());
    ui->zAxisComboBox->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickAxes/") + m_axesNames[2]).toInt());
    ui->wAxisComboBox->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickAxes/") + m_axesNames[3]).toInt());
    ui->dAxisComboBox->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickAxes/") + m_axesNames[4]).toInt());
    ui->rAxisComboBox->setCurrentIndex(m_settings->value(m_joystick_name + QString("/joystickAxes/") + m_axesNames[5]).toInt());
    connect(ui->xAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->yAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->zAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->wAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->dAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
    connect(ui->rAxisComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAxesSettings(int)));
}

void JoystickSetupDialog::updateUi(Joystick joystick){
    ui->xAxisProgressBar->setValue(joystick.axes[0]);
    ui->yAxisProgressBar->setValue(joystick.axes[1]);
    ui->zAxisProgressBar->setValue(joystick.axes[2]);
    ui->wAxisProgressBar->setValue(joystick.axes[3]);
    ui->dAxisProgressBar->setValue(joystick.axes[4]);
    ui->rAxisProgressBar->setValue(joystick.axes[5]);
}
