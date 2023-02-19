#include "thrustersetupdialog.h"
#include "ui_thrustersetupdialog.h"

ThrusterSetupDialog::ThrusterSetupDialog(QWidget *parent) :
    QDialog(parent),
    m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "CfRD", "RovUI2", this))
  , ui(new Ui::ThrusterSetupDialog)

{
    ui->setupUi(this);
}

void ThrusterSetupDialog::doUpdateSliders(int pos, int ind){
    thrustersOverride[ind] = pos;
    emit thrustersOverridden(thrustersOverride);
}

void ThrusterSetupDialog::doUpdateInvert(bool state, int ind){
    invertOverride[ind] = state;
    emit invertsOverridden(invertOverride);
}

ThrusterSetupDialog::~ThrusterSetupDialog()
{
    delete ui;
}
