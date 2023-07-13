#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include "qcheckbox.h"
#include "qcombobox.h"
#include "qdialog.h"
#include "qlabel.h"
#include "qmap.h"
#include "qobjectdefs.h"
#include "qslider.h"
#include "rovcameracommunication.h"
#include "ui_camerasettings.h"
#include <QObject>

namespace Ui {
    class CameraSettings;
}

class CameraSettings : public QDialog {
    Q_OBJECT
  public:
    explicit CameraSettings(QDialog *parent = nullptr);

    ~CameraSettings();

  signals:
    void updateCameraSettings(QMap<QString, Setting>);
    void updateServo(int pos);

  public slots:
    void recieveCameraSettings(QMap<QString, Setting>);

  private slots:
    void updateSetting(QString name, int val);
    void updateValueLabel(QString name, int val);

  private:
    Ui::CameraSettings *ui;

    QMap<QString, Setting> settings;

    QMap<QString, QLabel *>    labels;
    QMap<QString, QLabel *>    valueLabels;
    QMap<QString, QSlider *>   sliders;
    QMap<QString, QComboBox *> comboboxes;
    QMap<QString, QCheckBox *> checkboxes;

    QList<QMetaObject::Connection> conns;
};

#endif // CAMERASETTINGS_H
