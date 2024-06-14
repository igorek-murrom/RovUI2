#include "cameralabel.h"

CameraLabel::CameraLabel(QWidget *parent) : QLabel(parent) {}
int CameraLabel::heightForWidth(int w) const {
    double numerator   = 16;
    double denominator = 9;
    return floor(static_cast<double>(w) * denominator / numerator);
}

int CameraLabel::widthForHeight(int w) const {
    double numerator   = 9;
    double denominator = 16;
    return floor(static_cast<double>(w) * denominator / numerator);
}
