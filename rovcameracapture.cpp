#include "rovcameracapture.h"
#include "qapplication.h"
#include "qcamera.h"
#include "qcameraimagecapture.h"
#include "qcamerainfo.h"
#include "qcameraviewfinder.h"
#include "qcameraviewfindersettings.h"
#include "qdebug.h"
#include "qfileinfo.h"
#include "qmediaencodersettings.h"
#include "qmediaplayer.h"
#include "qmediarecorder.h"
#include "qmultimedia.h"
#include "qscopedpointer.h"
#include "qurl.h"
#include "qvideowidget.h"
#include <regex>

RovCameraCapture::RovCameraCapture(QWidget *parent)
    : QWidget(parent), m_camera(new QCamera("10.3.141.79:5000")),
      m_recorder(new QMediaRecorder(m_camera.data())),
      m_cameraCapture(new QCameraImageCapture(m_camera.data())),
      m_mediaPlayer(new QMediaPlayer(this)), m_record() {
    // qDebug() << "Supported video codecs: " <<
    // m_recorder->supportedVideoCodecs()
    //          << "\nSupported containers: " <<
    //          m_recorder->supportedContainers()
    //          << "\nSupported buffer formats: "
    //          << m_cameraCapture->supportedBufferFormats()
    //          << "\nsupported image codecs: "
    //          << m_cameraCapture->supportedImageCodecs();

    QVideoEncoderSettings videoSettings = m_recorder->videoSettings();
    videoSettings.setQuality(QMultimedia::VeryHighQuality);
    videoSettings.setCodec("video/x-h264");
    m_recorder->setContainerFormat("video/x-matroska-3d");
    m_recorder->setVideoSettings(videoSettings);

    QImageEncoderSettings imageSettings = m_cameraCapture->encodingSettings();
    imageSettings.setCodec("image/jpeg");
    imageSettings.setQuality(QMultimedia::VeryHighQuality);
    m_cameraCapture->setCaptureDestination(
        QCameraImageCapture::CaptureToBuffer);
    m_cameraCapture->setEncodingSettings(imageSettings);

    connect(m_cameraCapture.data(), &QCameraImageCapture::imageCaptured, this,
            [this](int id, QImage img) {
                QTemporaryFile *screenshotFile =
                    new QTemporaryFile("file:///tmp/screenshot-XXXXXX.png");
                screenshotFile->open();
                screenshotFile->close();
                QString generatedName = screenshotFile->fileName();
                delete screenshotFile;

                QFile file(generatedName);
                file.open(QIODevice::WriteOnly);

                img.save(&file, "PNG");
                emit screenshotReady(file.fileName());
                qDebug() << "Saving screenshot into " + generatedName;
            });

    m_camera->searchAndLock();
}

void RovCameraCapture::setViewfinder(QVideoWidget *vf) {
    qDebug() << "Adding ViewFinder " << vf;
    m_mediaPlayer->setVideoOutput(vf);
    m_mediaPlayer->setMedia(QUrl("gst-pipeline: udpsrc multicast-iface=enp3s0f4u1u2u1 address=239.255.1.2 port=5000 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! queue ! rtpjpegdepay ! jpegparse ! jpegdec ! timeoverlay ! videoconvert ! xvimagesink name=\"qtvideosink\""));
}

void RovCameraCapture::startRecord() {
    m_camera->setCaptureMode(QCamera::CaptureMode(QCamera::CaptureVideo));
    QTemporaryFile *recordFile = new QTemporaryFile("/tmp/record-XXXXXX.mkv");
    recordFile->open();
    recordFile->close();
    QString generatedName = recordFile->fileName();
    recordFile->remove();
    delete recordFile;
    m_recorder->setOutputLocation(generatedName);
    qDebug() << m_recorder->errorString();
    qDebug() << "Generated file name: " << generatedName;
    m_recorder->record();
}

void RovCameraCapture::pauseRecord() { m_recorder->pause(); }

void RovCameraCapture::stopRecord() {
    m_recorder->stop();
    qDebug() << m_recorder->state();
    qDebug() << m_recorder->status();
    qDebug() << m_recorder->errorString();
    qDebug() << m_recorder->state();
    qDebug() << "recordingReady file name: "
             << m_recorder->outputLocation().toString();
    QFileInfo check_file(m_recorder->outputLocation().toString());

    qDebug() << "waiting for record to appear @ "
             << m_recorder->outputLocation().toString();
    // while (!check_file.exists()) {
    //     QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    // }
    // emit recordingReady(m_recorder->outputLocation().toString());
}

void RovCameraCapture::screenshot() {
    m_camera->setCaptureMode(QCamera::CaptureMode(QCamera::CaptureStillImage));
    m_cameraCapture->capture();
}

void RovCameraCapture::startCapture() {
    m_camera->load();
    m_camera->start();
    m_mediaPlayer->play();

}

void RovCameraCapture::stopCapture() {
    m_camera->stop();
    m_camera->unload();
    m_mediaPlayer->stop();
}

QString RovCameraCapture::getRecordInfo() {
    return QString::number(m_recorder->duration()) + QString(" ms, ") +
           QString::number(
               QFile(m_recorder->outputLocation().toString()).size() /
               1048576.0f) +
           QString(" MB");
}
