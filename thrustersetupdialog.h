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
    void doUpdateSliders(int, int);//pos,num
    void doUpdateInvert(bool, int);//state,num

signals:
    void thrustersOverridden(int[8]);
    void invertsOverridden(bool[8]);

private:

    QScopedPointer<QSettings> m_settings;

    Ui::ThrusterSetupDialog *ui;

    int thrustersOverride[8];
    bool invertOverride[8];
};

#endif // THRUSTERSETUPDIALOG_H
