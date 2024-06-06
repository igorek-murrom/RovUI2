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
#include <QDateTime>
#include <QDir>

RovCameraCapture::RovCameraCapture(QWidget *parent)
    : QWidget(parent),
      m_mediaPlayer(new QMediaPlayer(this))
{
    video_path = new QDir("/home/igor/Videos/RovUI2");
    if (!video_path->exists()) video_path->mkpath(".");
    video_folder = QDateTime::currentDateTime().toString("HH-mm-ss_dd-MM-yyyy");
    video_path->mkdir(video_folder);
}

void RovCameraCapture::setViewfinder(QVideoWidget *vf) {
    qDebug() << "Adding ViewFinder " << vf;
    m_mediaPlayer->setVideoOutput(vf);
    this->setSource();
}

void RovCameraCapture::setSource() {
    this->stopCapture();
    if (m_index_camera) m_source = "gst-pipeline: v4l2src device=/dev/video0 ! tee name=t \ t. ! decodebin ! videoconvert ! videoscale ! video/x-raw,format=RGB,media=video ! queue ! videoconvert ! xvimagesink name=\"qtvideosink\" sync=false \ t. ! queue ! videoconvert ! x264enc tune=zerolatency ! matroskamux";
    else m_source = "gst-pipeline: udpsrc address=192.168.1.4 port=5000 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! queue ! rtpjpegdepay ! jpegparse ! jpegdec ! timeoverlay ! videoconvert ! xvimagesink name=\"qtvideosink\"";
    m_mediaPlayer->setMedia(m_source);
}

void RovCameraCapture::startRecord() {
    m_videopath = video_path->absolutePath() + "/" + video_folder + "/" + QDateTime::currentDateTime().toString("HH-mm-ss_dd-MM-yyyy") + ".mkv";
    m_mediaPlayer->setMedia(QUrl(m_source.toString() + " ! filesink location=" + m_videopath));
    this->startCapture();
}

void RovCameraCapture::pauseRecord() { }

void RovCameraCapture::stopRecord() {
    m_mediaPlayer->setMedia(m_source);
    this->startCapture();
}

void RovCameraCapture::startCapture() {
    m_mediaPlayer->play();

}

void RovCameraCapture::stopCapture() {
    m_mediaPlayer->stop();
}

bool RovCameraCapture::isRun() {
    return m_mediaPlayer.data()->isVideoAvailable();
}
