#ifndef CAMERALABEL_H
#define CAMERALABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>

class CameraLabel : public QLabel
{
    Q_OBJECT
public:
    CameraLabel(QWidget *parent=nullptr);
    int heightForWidth(int w) const;
};

#endif // CAMERALABEL_H
