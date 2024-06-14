#include "rovcameracapture.h"

RovCameraCapture::RovCameraCapture(QWidget *parent)
    : QWidget(parent),
      pipeline(nullptr),
      filesink(nullptr),
      videosink(nullptr)
{
    gst_init(nullptr, nullptr);

    video_path = new QDir("/home/igor/Videos/RovUI2");
    if (!video_path->exists()) video_path->mkpath(".");
    video_folder = QDateTime::currentDateTime().toString("HH-mm-ss_dd-MM-yyyy");
    video_path->mkdir(video_folder);
}

RovCameraCapture::~RovCameraCapture() {
    if (pipeline) {
        gst_object_unref(pipeline);
    }
}

void RovCameraCapture::setViewfinder(QVideoWidget *vf) {
    qDebug() << "Adding ViewFinder " << vf;
    m_windowId = vf->winId();
    this->setSource();
}

void RovCameraCapture::setSource() {
    if (m_index_camera) {
        m_source = "v4l2src device=/dev/video2 ! tee name=t t. ! decodebin ! videoconvert ! videoscale ! video/x-raw,format=RGB,media=video ! queue ! videoconvert ! xvimagesink name=qtvideosink sync=false t. ! videoconvert ! x264enc tune=zerolatency key-int-max=60 ! matroskamux";
    } else {
        m_source = "udpsrc address=192.168.1.4 port=5000 ! tee name=t t. ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! queue ! rtpjpegdepay ! jpegparse ! jpegdec ! videoconvert ! xvimagesink name=qtvideosink t. ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! queue ! rtpjpegdepay ! jpegparse ! jpegdec ! videoconvert ! x264enc tune=zerolatency ! matroskamux";
    }
    this->setPipeline(m_source);
}

void RovCameraCapture::setPipeline(QString source, bool recording) {
    this->stopCapture();
    if (recording) {
        source += " ! filesink name=filesink";
    }
    pipeline = gst_parse_launch(source.toStdString().c_str(), nullptr);
    filesink = gst_bin_get_by_name(GST_BIN(pipeline), "filesink");
    videosink = gst_bin_get_by_name(GST_BIN(pipeline), "qtvideosink");

    if (videosink) {
        gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(videosink), m_windowId);
    }

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_callback, this);
    gst_object_unref(bus);
}

void RovCameraCapture::startRecord() {
    this->setPipeline(m_source, true);
    m_videopath = video_path->absolutePath() + "/" + video_folder + "/" + (m_index_camera ? "back_" : "front_") + QDateTime::currentDateTime().toString("HH-mm-ss_dd-MM-yyyy") + ".mkv";
    g_object_set(filesink, "location", m_videopath.toStdString().c_str(), nullptr);
    this->startCapture();
}

void RovCameraCapture::stopRecord() {
    if (pipeline) {
        gst_element_send_event(pipeline, gst_event_new_eos());
    }
}

void RovCameraCapture::startCapture() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    }
}

void RovCameraCapture::stopCapture() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = nullptr;
    }
}

bool RovCameraCapture::isRun() {
    return pipeline != nullptr;
}

gboolean RovCameraCapture::bus_callback(GstBus *bus, GstMessage *message, gpointer data) {
    RovCameraCapture *self = static_cast<RovCameraCapture *>(data);

    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_EOS:
        qDebug() << "End of stream";
        gst_element_set_state(self->pipeline, GST_STATE_NULL);
        gst_object_unref(self->pipeline);
        self->pipeline = nullptr;
        self->setSource();
        self->startCapture();
        break;
    case GST_MESSAGE_ERROR:
        GError *err;
        gchar *debug_info;
        gst_message_parse_error(message, &err, &debug_info);
        qDebug() << "Error received from element " << GST_OBJECT_NAME(message->src) << ": " << err->message;
        qDebug() << "Debugging information: " << (debug_info ? debug_info : "none");
        g_clear_error(&err);
        g_free(debug_info);
        break;
    default:
        break;
    }
    return TRUE;
}
