#include "ledindicator.h"

LEDIndicator::LEDIndicator(QWidget *parent) :
    QLabel(parent)
{
    setState(StateOk);
    setFixedSize(20, 20);
}

void LEDIndicator::setState(State state)
{
    switch (state) {
        case StateOk:
            setStyleSheet(okStyleSheet.arg(10));
        break;
        case StateWarn:
            setStyleSheet(warnStyleSheet.arg(10));
        break;
        case StateError:
            setStyleSheet(errStyleSheet.arg(10));
        break;
        case StateFatal:
        default:
        break;
    }
}
