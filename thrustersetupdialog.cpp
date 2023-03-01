#include "thrustersetupdialog.h"
#include "ui_thrustersetupdialog.h"

ThrusterSetupDialog::ThrusterSetupDialog(QWidget *parent) :
    QDialog(parent),
    m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "CfRD", "RovUI2", this))
  , ui(new Ui::ThrusterSetupDialog)

{
    ui->setupUi(this);
}

void ThrusterSetupDialog::createConnections(){
    connect(ui->thrusterSlider1, &QSlider::valueChanged, this, [this](int val){doUpdateSliders(1,val);});
    connect(ui->thrusterSlider2, &QSlider::valueChanged, this, [this](int val){doUpdateSliders(2,val);});
    connect(ui->thrusterSlider3, &QSlider::valueChanged, this, [this](int val){doUpdateSliders(3,val);});
    connect(ui->thrusterSlider4, &QSlider::valueChanged, this, [this](int val){doUpdateSliders(4,val);});
    connect(ui->thrusterSlider5, &QSlider::valueChanged, this, [this](int val){doUpdateSliders(5,val);});
    connect(ui->thrusterSlider6, &QSlider::valueChanged, this, [this](int val){doUpdateSliders(6,val);});
    connect(ui->thrusterSlider7, &QSlider::valueChanged, this, [this](int val){doUpdateSliders(7,val);});
    connect(ui->thrusterSlider8, &QSlider::valueChanged, this, [this](int val){doUpdateSliders(8,val);});

    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this, [this](int val){doUpdateInvert(1,val);});
    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this, [this](int val){doUpdateInvert(2,val);});
    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this, [this](int val){doUpdateInvert(3,val);});
    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this, [this](int val){doUpdateInvert(4,val);});
    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this, [this](int val){doUpdateInvert(5,val);});
    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this, [this](int val){doUpdateInvert(6,val);});
    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this, [this](int val){doUpdateInvert(7,val);});
    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this, [this](int val){doUpdateInvert(8,val);});

    connect(ui->overrideEnable, SIGNAL(QCheckBox::stateChanged), this, SLOT(doChangeOverrideStatus(int)));
}

void ThrusterSetupDialog::doUpdateSliders(int ind, int pos){
    thrustersOverride[ind] = pos;
    emit thrustersOverridden(thrustersOverride);
}

void ThrusterSetupDialog::doUpdateInvert(int ind, int state){
    invertOverride[ind] = state;
    emit invertsOverridden(invertOverride);
}

void ThrusterSetupDialog::doChangeOverrideStatus(int state){
    emit overrideStatusChanged(state>0);
}

ThrusterSetupDialog::~ThrusterSetupDialog()
{
    delete ui;
}
