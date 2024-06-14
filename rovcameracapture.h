#ifndef ROVCAMERACAPTURE_H
#define ROVCAMERACAPTURE_H

#include <QApplication>
#include <QDebug>
#include <QMultimedia>
#include <QScopedPointer>
#include <QUrl>
#include <QVideoWidget>
#include <QDateTime>
#include <QDir>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

class RovCameraCapture : public QWidget {
    Q_OBJECT

  public:
    RovCameraCapture(QWidget *parent = nullptr);
    ~RovCameraCapture();

    void setViewfinder(QVideoWidget *vf);
    bool isRun();
    void setSource();
    void startCapture();
    void stopCapture();
    bool m_index_camera = true;

  public slots:
    void startRecord();
    void stopRecord();

  private:
    static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer data);
    void setPipeline(QString source, bool recording=false);
    GstElement *pipeline;
    GstElement *filesink;
    GstElement *videosink;
    QString m_source;
    QString m_videopath;
    QDir *video_path;
    QString video_folder;
    WId m_windowId;
};

#endif // ROVCAMERACAPTURE_H
