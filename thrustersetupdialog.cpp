#include "thrustersetupdialog.h"
#include "helpers.h"
#include "qdebug.h"
#include "ui_thrustersetupdialog.h"

ThrusterSetupDialog::ThrusterSetupDialog(QWidget *parent)
    : QDialog(parent),
      m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope,
                               "CfRD", "RovUI2", this)),
      ui(new Ui::ThrusterSetupDialog)

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

void ThrusterSetupDialog::createConnections() {
    connect(ui->thrusterSlider1, &QSlider::valueChanged, this,
            [this](int val) { updateSliders(1, val); });
    connect(ui->thrusterSlider2, &QSlider::valueChanged, this,
            [this](int val) { updateSliders(2, val); });
    connect(ui->thrusterSlider3, &QSlider::valueChanged, this,
            [this](int val) { updateSliders(3, val); });
    connect(ui->thrusterSlider4, &QSlider::valueChanged, this,
            [this](int val) { updateSliders(4, val); });
    connect(ui->thrusterSlider5, &QSlider::valueChanged, this,
            [this](int val) { updateSliders(5, val); });
    connect(ui->thrusterSlider6, &QSlider::valueChanged, this,
            [this](int val) { updateSliders(6, val); });
    connect(ui->thrusterSlider7, &QSlider::valueChanged, this,
            [this](int val) { updateSliders(7, val); });
    connect(ui->thrusterSlider8, &QSlider::valueChanged, this,
            [this](int val) { updateSliders(8, val); });

    connect(ui->thrusterInvert1, &QCheckBox::stateChanged, this,
            [this](int val) { updateInvert(1, val); });
    connect(ui->thrusterInvert2, &QCheckBox::stateChanged, this,
            [this](int val) { updateInvert(2, val); });
    connect(ui->thrusterInvert3, &QCheckBox::stateChanged, this,
            [this](int val) { updateInvert(3, val); });
    connect(ui->thrusterInvert4, &QCheckBox::stateChanged, this,
            [this](int val) { updateInvert(4, val); });
    connect(ui->thrusterInvert5, &QCheckBox::stateChanged, this,
            [this](int val) { updateInvert(5, val); });
    connect(ui->thrusterInvert6, &QCheckBox::stateChanged, this,
            [this](int val) { updateInvert(6, val); });
    connect(ui->thrusterInvert7, &QCheckBox::stateChanged, this,
            [this](int val) { updateInvert(7, val); });
    connect(ui->thrusterInvert8, &QCheckBox::stateChanged, this,
            [this](int val) { updateInvert(8, val); });

    connect(ui->overrideEnable, SIGNAL(stateChanged(int)), this,
            SLOT(changeOverrideStatus(int)));
}

void ThrusterSetupDialog::updateSliders(int ind, int pos) {
    thrustersOverride[ind - 1] = pos;
    m_dataLabels[ind - 1]->setText(QString::number(pos));
    emit thrustersOverridden(thrustersOverride);
}

void ThrusterSetupDialog::updateInvert(int ind, int state) {
    state ? BIT_SET(invertOverride, ind) : BIT_CLEAR(invertOverride, ind);
    emit invertsOverridden(invertOverride);
}

void ThrusterSetupDialog::changeOverrideStatus(int state) {
    emit overrideStatusChanged(state > 0);
}

ThrusterSetupDialog::~ThrusterSetupDialog() { delete ui; }
