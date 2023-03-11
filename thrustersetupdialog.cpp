#include "thrustersetupdialog.h"
#include "helpers.h"
#include "qdebug.h"
#include "ui_thrustersetupdialog.h"

ThrusterSetupDialog::ThrusterSetupDialog(QWidget *parent) :
    QDialog(parent),
    m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "CfRD", "RovUI2", this))
  , ui(new Ui::ThrusterSetupDialog)

{
    ui->setupUi(this);
    m_dataLabels[0] = ui->thrusterDLabel1;
    m_dataLabels[1] = ui->thrusterDLabel2;
    m_dataLabels[2] = ui->thrusterDLabel3;
    m_dataLabels[3] = ui->thrusterDLabel4;
    m_dataLabels[4] = ui->thrusterDLabel5;
    m_dataLabels[5] = ui->thrusterDLabel6;
    m_dataLabels[6] = ui->thrusterDLabel7;
    m_dataLabels[7] = ui->thrusterDLabel8;

    createConnections();
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

    connect(ui->overrideEnable, SIGNAL(stateChanged(int)), this, SLOT(doChangeOverrideStatus(int)));
}

void ThrusterSetupDialog::doUpdateSliders(int ind, int pos){
    thrustersOverride[ind] = pos;
    qDebug() << "pos: " << QString::number(pos) << thrustersOverride[ind];
    m_dataLabels[ind-1]->setText(QString::number(pos));
    emit thrustersOverridden(thrustersOverride);
}

void ThrusterSetupDialog::doUpdateInvert(int ind, int state){
    state ? BIT_SET(invertOverride, ind) : BIT_CLEAR(invertOverride, ind);
    emit invertsOverridden(invertOverride);
}

void ThrusterSetupDialog::doChangeOverrideStatus(int state){
    emit overrideStatusChanged(state>0);
}

ThrusterSetupDialog::~ThrusterSetupDialog()
{
    delete ui;
}
