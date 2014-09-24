#include "scannercontroller.h"
#include <QPixmap>

ScannerController::ScannerController(QObject *parent) :
    QObject(parent),camNumber_(0),scandistance_(250),stepsize_(1),framerate_(5),width_(1920),height_(1080),dist(0)
{

    QTimer* timer_ = new QTimer();
    timer_->setInterval(1000.0/framerate_);

    capwebcam(camNumber_);


}

void ScannerController::ScanStep(){
    capwebcam.read(matOriginal);
    if(matOriginal.empty()==true){
        emit error(QString("Error reading camera at position:")+QString::number(position_));
    }
    cv::Mat dest;
    cv::cvtColor(matOriginal, dest,CV_BGR2RGB);
    QPixmap m = QPixmap::fromImage(QImage((unsigned char*) dest.data,dest.cols,dest.rows,dest.step,QImage::Format_RGB888));
}
