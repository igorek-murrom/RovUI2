#ifndef ROVCAMERACAPTURE_H
#define ROVCAMERACAPTURE_H

#include <QTemporaryFile>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraViewfinderSettings>
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QMediaRecorder>
#include <QScopedPointer>
#include <QTimer>
#include <QVariant>
#include <QWidget>
#include <cstdint>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

/**
 * \brief The RovCameraCapture class
 */
class RovCameraCapture : public QWidget {
    Q_OBJECT
  public:
    /**
     * \brief Default consrtuctor
     * \param parent Parent
     */
    RovCameraCapture(QWidget *parent);

    QString getRecordInfo();

    void setViewfinder(QCameraViewfinder *viewfinder);

  signals:
    /**
     * \brief Emitted on processed image change
     * \param image Processed image
     */
    void imgProcessed(QImage image);

    void screenshotReady(QString imagePath);

    void recordingReady(QString recordPath);

  public slots:
    /**
     * \brief Starts video capture from the specified index
     */
    void startCapture();

    /**
     * \brief Stops capture and closes the stream
     */
    void stopCapture();

    void startRecord();

    void pauseRecord();

    void stopRecord();

    void screenshot();

  private slots:
  private:
    /**
     * \brief Set to true when capture is set to video mode
     */
    bool updateNeeded = true;

    bool recording;

    bool screenshotNeeded;

    /**
     * \brief OpenCV video capture
     */
    cv::VideoCapture cap;

    QScopedPointer<QCamera> m_camera;

    QScopedPointer<QMediaRecorder> m_recorder;

    QScopedPointer<QCameraImageCapture> m_cameraCapture;


    /**
     * \brief Runs processCamera() every 16.6 msecs
     */
    QTimer *m_displayTimer;

    QList<QImage> m_record;

    int32_t m_recordSize = 0;

    int32_t m_recordLength = 0;
};

#endif // ROVCAMERACAPTURE_H
