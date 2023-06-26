#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include "qcheckbox.h"
#include "qcombobox.h"
#include "qdialog.h"
#include "qlist.h"
#include "qobjectdefs.h"
#include "qslider.h"
#include "rovcameracommunication.h"
#include "ui_camerasettings.h"
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui {
    class CameraSettings;
}
QT_END_NAMESPACE

class cameraSettings : public QDialog {
    Q_OBJECT
  public:
    explicit cameraSettings(QDialog *parent = nullptr);

  signals:
    void updateCameraSettings();

  public slots:
    void recieveCameraSettings(QMap<QString, Setting>);

  private:
    Ui::CameraSettings              *ui;
    QMap<QString, Setting>           settings;
    QList<QSlider *>                 sliders;
    QList<QComboBox *>               comboboxes;
    QList<QCheckBox *>               checkboxes;
    QList<QMetaObject::Connection> conns;
};

#endif // CAMERASETTINGS_H
