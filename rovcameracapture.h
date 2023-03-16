#ifndef ROVCAMERACAPTURE_H
#define ROVCAMERACAPTURE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

class RovCameraCapture : public QWidget {
    Q_OBJECT
public:
    RovCameraCapture(QWidget* parent);

signals:
    void imgProcessed(QImage);

public slots:
    void startCapture(int);
    void stopCapture();

private slots:
    void doProcessCamera();

private:
    bool updateNeeded = true;

    cv::VideoCapture cap;

    QScopedPointer<QLabel> m_camLabel;

    QTimer *displayTimer;
};

#endif // ROVCAMERACAPTURE_H
