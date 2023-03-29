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

/*!
 * \brief The RovCameraCapture class
 */
class RovCameraCapture : public QWidget {
    Q_OBJECT
public:
    /*!
     * \brief Default consrtuctor
     * \param parent Parent
     */
    RovCameraCapture(QWidget* parent);

signals:
    /*!
     * \brief Emitted on processed image change
     * \param image Processed image
     */
    void imgProcessed(QImage image);

public slots:
    /*!
     * \brief Starts video capture from the specified index
     * \param index Index of capture
     */
    void startCapture(int index);

    /*!
     * \brief Stops capture and closes the stream
     */
    void stopCapture();

private slots:
    /*!
     * \brief Process camera capture
     */
    void processCamera();

private:
    /*!
     * \brief Set to true when capture is set to video mode
     */
    bool updateNeeded = true;

    /*!
     * \brief OpenCV video capture
     */
    cv::VideoCapture cap;

    /*!
     * \brief Runs processCamera() every 16.6 msecs
     */
    QTimer *displayTimer;
};

#endif // ROVCAMERACAPTURE_H
