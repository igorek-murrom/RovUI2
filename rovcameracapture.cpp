#include "rovcameracapture.h"

RovCameraCapture::RovCameraCapture(QWidget* parent) :
    QWidget(parent)
{
    displayTimer = new QTimer(this);
    displayTimer->setInterval(16);
    displayTimer->start();
    connect(displayTimer, &QTimer::timeout, this, &RovCameraCapture::doProcessCamera);
}

void RovCameraCapture::doStartCapture(){

    qWarning() << "Opening result: " << QString(cap.open(0));
}

void RovCameraCapture::doStopCapture(){
    cap.release();
}

void RovCameraCapture::createConnections(){
}

void RovCameraCapture::doProcessCamera(){
    if(cap.isOpened()){
        updateNeeded = true;
        cv::Mat img;
        cap.read(img);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        QImage qImg((uchar*)img.data, img.cols,
            img.rows, img.step, QImage::Format_RGB888);
        emit imgProcessed(qImg);
    }
    else{
        if(updateNeeded){
            QImage qImg(":/images/placeholder.jpg");
            emit imgProcessed(qImg);
            updateNeeded = false;
        }
    }
}
