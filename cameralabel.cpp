#include "cameralabel.h"

CameraLabel::CameraLabel(QWidget *parent) : QLabel(parent){

}
int CameraLabel::heightForWidth(int w) const{
    double numerator = 16;
    double denominator = 9;
    return static_cast<int>(static_cast<double>(w) * denominator / numerator );
}
