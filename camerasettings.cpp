#include "camerasettings.h"
#include "ui_camerasettings.h"

CameraSettings::CameraSettings(QDialog *parent)
    : QDialog{parent}, ui(new Ui::CameraSettings) {
    ui->setupUi(this);

    connect(ui->buttonBox->buttons()[0], &QAbstractButton::clicked, this,
            [this] { emit updateCameraSettings(settings); });
    connect(ui->buttonBox->buttons()[2], &QAbstractButton::clicked, this,
            [this] { emit updateCameraSettings(settings); });
    connect(ui->servoSlider, &QSlider::sliderMoved, this, [this](int pos) {
        ui->servoPositionLabel->setText(QString::number(pos));
        emit updateServo(pos);
    });
}

CameraSettings::~CameraSettings() { delete ui; }

void CameraSettings::recieveCameraSettings(QMap<QString, Setting> map) {
    for (QMetaObject::Connection conn : conns)
        disconnect(conn);
    for (QSlider *slider : sliders)
        delete slider;
    for (QComboBox *combobox : comboboxes)
        delete combobox;
    for (QCheckBox *checkbox : checkboxes)
        delete checkbox;
    for (QLabel *label : labels)
        delete label;
    for (QLabel *label : valueLabels)
        delete label;

    for (QString k : map.keys()) {
        if (map[k].menu.isEmpty()) {
            QSlider *slider = new QSlider(Qt::Horizontal);
            slider->setMaximum(map[k].maxValue);
            slider->setMinimum(map[k].minValue);
            slider->setSliderPosition(map[k].currentValue);
            slider->setSingleStep(1);
            slider->setMinimumWidth(70);
            sliders.insert(k, slider);
            conns.append(connect(slider, &QSlider::sliderMoved, this,
                                 [this, k](int val) {
                                     updateSetting(k, val);
                                     updateValueLabel(k, val);
                                 }));
        } else {
            QComboBox *combobox = new QComboBox(this);
            for (QString menuItem : map[k].menu.keys()) {
                combobox->addItem(menuItem, map[k].menu[menuItem]);
            }
            comboboxes.insert(k, combobox);
            conns.append(connect(
                combobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this, k, combobox](int val) {
                    updateSetting(k, combobox->itemData(val).toInt());
                }));
        }
    }
    int row = 0;
    for (QString name : sliders.keys()) {
        QLabel *label      = new QLabel(name);
        QLabel *valueLabel = new QLabel("0");
        valueLabel->setMinimumWidth(50);

        labels.insert(name, label);
        valueLabels.insert(name, valueLabel);

        ui->gridLayoutInner->addWidget(label, row, 0);
        ui->gridLayoutInner->addWidget(sliders[name], row, 2);
        ui->gridLayoutInner->addWidget(valueLabel, row, 1);

        updateValueLabel(name, map[name].currentValue);

        row++;
    }

    for (QString name : comboboxes.keys()) {
        QLabel *label = new QLabel(name);

        labels.insert(name, label);

        ui->gridLayoutInner->addWidget(label, row, 0);
        ui->gridLayoutInner->addWidget(comboboxes[name], row, 2);

        row++;
    }

    for (QString name : checkboxes.keys()) {
        QLabel *label = new QLabel(name);

        labels.insert(name, label);

        ui->gridLayoutInner->addWidget(label, row, 0);
        ui->gridLayoutInner->addWidget(valueLabels[name], row, 1);
        ui->gridLayoutInner->addWidget(comboboxes[name], row, 2);

        row++;
        updateValueLabel(name, map[name].currentValue);
    }
}

void CameraSettings::updateSetting(QString name, int value) {
    settings[name].currentValue = value;
}

void CameraSettings::updateValueLabel(QString name, int value) {
    valueLabels[name]->setText(QString::number(value));
}
