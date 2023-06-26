#include "camerasettings.h"
#include "qabstractbutton.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "qobjectdefs.h"
#include "qslider.h"

cameraSettings::cameraSettings(QDialog *parent) : QDialog{parent} {
    ui->setupUi(this);
    connect(ui->buttonBox->buttons()[0], &QAbstractButton::click, this,
            [this] { emit updateCameraSettings(); });
    connect(ui->buttonBox->buttons()[1], &QAbstractButton::click, this,
            [this] { emit updateCameraSettings(); });
}

void cameraSettings::recieveCameraSettings(QMap<QString, Setting> map) {
    for (QMetaObject::Connection conn : conns)
        disconnect(conn);
    for (QSlider *slider : sliders)
        delete slider;
    for (QComboBox *combobox : comboboxes)
        delete combobox;
    for (QCheckBox *checkbox : checkboxes)
        delete checkbox;

    for (QString k : map.keys()) {
        if (map[k].menu.isEmpty()) {
            QSlider *slider = new QSlider;
            slider->setMaximum(map[k].maxValue);
            slider->setMinimum(map[k].minValue);
            slider->setSingleStep(1);
            sliders.append(slider);
            connect(slider, SIGNAL(sliderMoved(int position)), this, )
        } else {
            QComboBox *combobox = new QComboBox;
            for (auto menuItem : map[k].menu.keys()) {
                combobox->addItem(menuItem, map[k].menu[menuItem]);
            }
        }
    }
}