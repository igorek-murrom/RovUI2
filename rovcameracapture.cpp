#include "rovcameracapture.h"
#include "qcamera.h"
#include "qcameraimagecapture.h"
#include "qcamerainfo.h"
#include "qcameraviewfinder.h"
#include "qcameraviewfindersettings.h"
#include "qdebug.h"
#include "qmediaencodersettings.h"
#include "qmediarecorder.h"
#include "qmultimedia.h"
#include "qscopedpointer.h"
#include "qurl.h"
#include <regex>

RovCameraCapture::RovCameraCapture(QWidget *parent)
    : QWidget(parent),
      m_camera(new QCamera(QCameraInfo::availableCameras().last())),
      m_recorder(new QMediaRecorder(m_camera.data())),
      m_cameraCapture(new QCameraImageCapture(m_camera.data())), m_record() {
    // qDebug() << "Supported video codecs: " << m_recorder->supportedVideoCodecs()
    //          << "\nSupported containers: " << m_recorder->supportedContainers()
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
                    new QTemporaryFile("/tmp/screenshot-XXXXXX.png");
                screenshotFile->open();
                screenshotFile->close();
                QString generatedName = screenshotFile->fileName();
                delete screenshotFile;

                QFile file(generatedName);
                file.open(QIODevice::WriteOnly);
                qDebug() << "Saving screenshot into " + generatedName;

                img.save(&file, "PNG");
                emit screenshotReady(file.fileName());
            });

    m_camera->searchAndLock();
}

void RovCameraCapture::setViewfinder(QCameraViewfinder *vf) {
    m_camera->setViewfinder(vf);
}

void RovCameraCapture::startRecord() {
    m_camera->setCaptureMode(QCamera::CaptureMode(QCamera::CaptureVideo));
    QTemporaryFile *recordFile = new QTemporaryFile("/tmp/record-XXXXXX.mkv");
    recordFile->open();
    recordFile->close();
    QString generatedName = recordFile->fileName();
    delete recordFile;
    m_recorder->setOutputLocation(generatedName);
    m_recorder->record();
}

void RovCameraCapture::pauseRecord(){
    m_recorder->pause();
}

void RovCameraCapture::stopRecord() {
    m_recorder->stop();
    // m_recorder->state()
    // emit recordingReady(m_recorder->outputLocation().toString());
}

void RovCameraCapture::screenshot() {
    m_camera->setCaptureMode(QCamera::CaptureMode(QCamera::CaptureStillImage));
    m_cameraCapture->capture();
}

void RovCameraCapture::startCapture() {
    m_camera->load();
    m_camera->start();
}

void RovCameraCapture::stopCapture() {
    m_camera->stop();
    m_camera->unload();
}

QString RovCameraCapture::getRecordInfo() {
    return QString::number(m_recorder->duration()) + QString(" ms, ") +
           QString::number(
               QFile(m_recorder->outputLocation().toString()).size() /
               1048576.0f) +
           QString(" MB");
}
