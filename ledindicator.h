#ifndef LEDINDICATOR_H
#define LEDINDICATOR_H

#include <QLabel>
#include <QObject>
#include <QWidget>

class LEDIndicator : public QLabel
{
    Q_OBJECT
public:
    enum State{
        StateOk,
        StateWarn,
        StateError,
        StateFatal
    };

    explicit LEDIndicator(QWidget *parent = 0);
    void setState(State state);

private:
    const QString okStyleSheet = QString("color: white;"
                                         "border-radius: %1;"
                                         "border: 1.4px solid white;"
                                         "background-color: qlineargradient("
                                         "spread:pad, "
                                         "x1:0.145, y1:0.16, "
                                         "x2:1, y2:1, "
                                         "stop:0 rgba(20, 252, 7, 255), "
                                         "stop:1 rgba(25, 134, 5, 255)"
                                         ");");
    const QString warnStyleSheet = QString("color: white;"
                                           "border-radius: %1;"
                                           "border: 1.4px solid white;"
                                           "background-color: qlineargradient("
                                           "spread:pad, "
                                           "x1:0.232, y1:0.272, "
                                           "x2:0.98, y2:0.959773, "
                                           "stop:0 rgba(255, 113, 4, 255), "
                                           "stop:1 rgba(91, 41, 7, 255))");
    const QString errStyleSheet = QString("color: white;"
                                          "border-radius: %1;"
                                          "border: 1.4px solid white;"
                                          "background-color: qlineargradient("
                                          "spread:pad, "
                                          "x1:0.145, y1:0.16, "
                                          "x2:0.92, y2:0.988636, "
                                          "stop:0 rgba(255, 12, 12, 255), "
                                          "stop:0.869347 rgba(103, 0, 0, 255));");
//    const QString blueSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.04, y1:0.0565909, x2:0.799, y2:0.795, stop:0 rgba(203, 220, 255, 255), stop:0.41206 rgba(0, 115, 255, 255), stop:1 rgba(0, 49, 109, 255));").arg(SIZE/2);



};

#endif // LEDINDICATOR_H
