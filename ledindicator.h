#ifndef LEDINDICATOR_H
#define LEDINDICATOR_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QDebug>

/*!
 * \brief The LEDIndicator class will be used to indicate ROV status when the heartbeat protocol will be implemented, for now it's just a stub
 */
class LEDIndicator : public QLabel
{
    Q_OBJECT
public:
    /*!
     * \brief The State enum is used to set the state of the indicator
     */
    enum State{
        StateOk,
        StateWarn,
        StateError,
        StateFatal
    };

    /*!
     * \brief Explicit defult constructor
     * \param parent Parent
     */
    explicit LEDIndicator(QWidget *parent = 0);

    /*!
     * \brief Sets state of the indicator
     * \param state State to set
     */
    void setState(State state);

private:
    /*!
     * \brief Style sheet for the "Ok" status: green
     */
    const QString okStyleSheet = QString("color: white;"
                                         "border-radius: %1;"
                                         "border: 0px;"
                                         "background-color: qlineargradient("
                                         "spread:pad, "
                                         "x1:0.145, y1:0.16, "
                                         "x2:1, y2:1, "
                                         "stop:0 rgba(20, 252, 7, 255), "
                                         "stop:1 rgba(25, 134, 5, 255)"
                                         ");");
    /*!
     * \brief Style sheet for the "Warn" status: orange
     */
    const QString warnStyleSheet = QString("color: white;"
                                           "border-radius: %1;"
                                           "border: 1.4px solid white;"
                                           "background-color: qlineargradient("
                                           "spread:pad, "
                                           "x1:0.232, y1:0.272, "
                                           "x2:0.98, y2:0.959773, "
                                           "stop:0 rgba(255, 113, 4, 255), "
                                           "stop:1 rgba(91, 41, 7, 255))");
    /*!
     * \brief Style sheet for the "Err" status: red
     */
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
