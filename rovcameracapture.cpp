#include "rovcameracapture.h"

RovCameraCapture::RovCameraCapture(QWidget* parent) :
    QWidget(parent)
{
    displayTimer = new QTimer(this);
    displayTimer->setInterval(16 + 2/3);
    displayTimer->start();
    connect(displayTimer, &QTimer::timeout, this, &RovCameraCapture::processCamera);
}

void RovCameraCapture::startCapture(int index){

    qWarning() << "Opening result: " << QString(cap.open("/dev/video2"));
    updateNeeded = true;
}

void RovCameraCapture::stopCapture(){
    cap.release();
}

void RovCameraCapture::processCamera(){
    if(cap.isOpened()){
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
