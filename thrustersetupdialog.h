#ifndef THRUSTERSETUPDIALOG_H
#define THRUSTERSETUPDIALOG_H

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
    void thrustersOverridden(int[8]);
    void invertsOverridden(bool[8]);

private:
    void createConnections();

    QScopedPointer<QSettings> m_settings;

    Ui::ThrusterSetupDialog *ui;

    int thrustersOverride[8];
    bool invertOverride[8];
};

#endif // THRUSTERSETUPDIALOG_H
