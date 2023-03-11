#ifndef THRUSTERSETUPDIALOG_H
#define THRUSTERSETUPDIALOG_H

#include "qlabel.h"
#include <QDialog>
#include <QSettings>

namespace Ui {
class ThrusterSetupDialog;
}

class ThrusterSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThrusterSetupDialog(QWidget *parent = nullptr);
    ~ThrusterSetupDialog();

public slots:
    void doUpdateSliders(int, int);//state,num
    void doUpdateInvert(int, int);//state,num
    void doChangeOverrideStatus(int);

signals:
    void overrideStatusChanged(bool);
    void thrustersOverridden(QList<qint8>);
    void invertsOverridden(qint8);

private:
    void createConnections();

    QScopedPointer<QSettings> m_settings;
    QLabel *m_dataLabels[8];

    Ui::ThrusterSetupDialog *ui;

    QList<qint8> thrustersOverride = {0,0,0,0,0,0,0,0};
    qint8 invertOverride;
};

#endif // THRUSTERSETUPDIALOG_H
