#ifndef ROVCAMERACAPTURE_H
#define ROVCAMERACAPTURE_H

#include "qscopedpointer.h"
#include "qvideowidget.h"
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
#include <QTemporaryFile>
#include <QTimer>
#include <QVariant>
#include <QWidget>
#include <QMediaPlayer>
#include <cstdint>

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

    void setViewfinder(QVideoWidget *viewfinder);

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
    QScopedPointer<QCamera> m_camera;

    QScopedPointer<QMediaRecorder> m_recorder;

    QScopedPointer<QCameraImageCapture> m_cameraCapture;
    QScopedPointer<QMediaPlayer> m_mediaPlayer;


    /**
     * \brief Runs processCamera() every 16.6 msecs
     */
    QTimer *m_displayTimer;

    QList<QImage> m_record;

    int32_t m_recordSize = 0;

    int32_t m_recordLength = 0;
};

#endif // ROVCAMERACAPTURE_H
