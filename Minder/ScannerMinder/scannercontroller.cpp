#include "scannercontroller.h"
#include <QPixmap>
#include <QDebug>

ScannerController::ScannerController(QObject *parent) :
    QObject(parent),camNumber_(0),scandistance_(250),stepsize_(1),framerate_(5),width_(1920),height_(1080),dist(0)
{
    sai_=  new ScannerArduinoInterface();
    timer_ = new QTimer();
    timer_->setInterval(1000.0/framerate_);
    connect(timer_,SIGNAL(timeout()),this,SLOT(ScanStep()));




}

void ScannerController::portSelected(QString port){
    delete sai_;
    capwebcam.open(camNumber_);
    bool state=true;
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_WIDTH,width_);
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_HEIGHT,height_);

    if(!state){
        qDebug()<< QString("Error setting capture sizes)");
    }


    sai_=  new ScannerArduinoInterface(port,BAUD9600);
    connect(sai_,SIGNAL(buttonPressed()),this,SLOT(buttonPress()));
    connect(sai_,SIGNAL(errored()),this,SLOT(scannerError()));
    connect(sai_,SIGNAL(scanMovementCompleted()),this,SLOT(scanComplete()));
}

void ScannerController::buttonPress(){
    sai_->startScan();
    timer_->start();
    dist=0;
}
void ScannerController::scannerError(){}
void ScannerController::scanComplete(){}

void ScannerController::ScanStep(){

    capwebcam.read(matOriginal);


    dist += stepsize_;

    if(matOriginal.empty()==true){
        qDebug()<<QString("Error reading camera at position:")<<QString::number(dist);
    }

    cv::Mat dest;
    cv::cvtColor(matOriginal, dest,CV_BGR2RGB);

    QImage m = QImage((unsigned char*) dest.data,dest.cols,dest.rows,dest.step,QImage::Format_RGB888);
    m.save(QString::number(dist)+".jpeg","JPEG");
    if (dist>scandistance_){
        timer_->stop();
    }

}