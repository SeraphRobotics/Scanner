#include "scancontroller.h"
#include <QDomDocument>
#include <QDebug>

ScanController::ScanController(QObject *parent):QObject(parent),camNumber_(-1),
    axis_(""),ready_(0),scandistance_(0),
    stepsize_(0),framerate_(0),position_(0),targetposition_(0),xvector_(QVector<float>(3,0)),width_(1920),height_(1080)
{    
    timer_ = new QTimer();
    vm_ = new VirtualPrinter();
    connect(timer_,SIGNAL(timeout()),this,SLOT(ScanStep()));
    connect(this,SIGNAL(scanComplete()),timer_,SLOT(stop()));

    // Used in motion commands durring scan
    axes_["x"]=0;
    axes_["y"]=1;
    axes_["z"]=2;

}

void ScanController::loadVM(VirtualPrinter* vm)
{
    vm_ = vm;
    isReady();
}

void ScanController::loadScanData(ScanData* sd){
    SD_=sd;
}


void ScanController::setWidth(int w){
    width_=w;
}

void ScanController::setHeight(int h){
    height_=h;
}

void ScanController::setCamera(int c)
{
    camNumber_ = c;

    capwebcam.open(c);

    if (capwebcam.isOpened()==false){
        emit error(QString("Error: capwebcam not accessed succesfully"));
        return;
    }

    bool state=true;
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_WIDTH,width_);
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_HEIGHT,height_);

    if(!state){
        emit error(QString("Error setting capture sizes)"));
    }
    isReady();
}

void ScanController::setAxis(QString Axis)
{
    axis_ = Axis.toLower();
    isReady();
}


bool ScanController::isReady()
{
    QString db;
    // ready only if webcam works, vm ready, and values are set
    if((capwebcam.isOpened()==false)||(vm_->isInitialized()==false)||(axis_=="")||(scandistance_==0)||(stepsize_==0)||(framerate_==0)){//||(vm_==NULL)
        ready_=false;
        db=QString("\nNot ready: ")+QString("VM is ")+QString(vm_->isInitialized() ? "Initialized":"Not Initialized")+
                  QString(", Webcam: ") +QString(capwebcam.isOpened() ? "is opened":"is not opened")+
                  QString(", Axis: ")+QString(axis_)+QString(", ScanDistance: ")+
                  QString::number(scandistance_)+QString(", StepSize: ")+
                  QString::number(stepsize_)+QString(", scanspeed: ")+QString::number(framerate_);
        qDebug()<<db;
        emit error(db);
    }else{
        ready_=true;
        db="ready";
        emit updateStatus(db);
    }
    return ready_;
}


void ScanController::setScan(float scandistance, float stepsize, float  framerate)
{
    scandistance_=scandistance;
    stepsize_=stepsize;
    framerate_=framerate;
    timer_->setInterval(1);
    isReady();
}

void ScanController::StartScan()
{
    if(ready_){
        vm_->resetPosition();

        QVector<double> pos = vm_->currentPosition();
        position_= pos[axes_[axis_]];
        emit updateStatus(QString("Start Position: ")+QString::number(position_));


        xvector_[0]=0;
        xvector_[1]=0;
        xvector_[2]=0;
        xvector_[axes_[axis_]]=scandistance_;

        //vm_->xyzmotion->setAcceleration(100);
        float speed = framerate_*stepsize_;

        vm_->move(xvector_[0],xvector_[1],xvector_[2],speed);

        timer_->start();
        targetposition_=position_+stepsize_;
        emit scanRunning(true);
    }else{
        emit error(QString("not ready"));
    }
}

void ScanController::StopScan()
{
    timer_->stop();
    vm_->stop();

    // Return to Origin
    float speed = framerate_*stepsize_;
    QVector<double> p=vm_->currentPosition();
    vm_->move(-p[0],-p[1],-p[2],speed);
    emit updateStatus("Writing to disk");
//    SD_->writeToDisk();
    emit updateStatus("Writing complete");
    vm_->waitMove();
    emit scanRunning(false);
}



void ScanController::ScanStep()
{
    if(!ready_){
        emit error(QString("Scanner not ready"));
        return;
    }

    QVector<double> pos = vm_->currentPosition();
    position_= pos[axes_[axis_]];
    float tolerance=0.1;
    float p_error=position_-targetposition_;

    qDebug()<<"position: "<<position_<<"error: "<<p_error<<"\ttarget:"<<targetposition_;

    if(fabs(p_error)<tolerance || (position_>targetposition_)){
        targetposition_+=stepsize_;
        ///CAPTURE DATA////
        capwebcam.read(matOriginal);
        if(matOriginal.empty()==true){
            emit error(QString("Error reading camera at position:")+QString::number(position_));
        }
        cv::Mat dest;
        cv::cvtColor(matOriginal, dest,CV_BGR2RGB);
        QPixmap m = QPixmap::fromImage(QImage((unsigned char*) dest.data,dest.cols,dest.rows,dest.step,QImage::Format_RGB888));
        SD_->addImage(position_,m);

        emit update(position_);
        emit updateStatus(QString("Position is now ")+QString::number(position_));
    }
    //If the error is greated than the distance between targets, increate the target
    if(p_error>stepsize_){targetposition_+=stepsize_;}

    if(targetposition_>scandistance_){
        QString db = QString("DONE");
        emit updateStatus(db);
        StopScan();
        emit scanComplete();
    }
}

ScanData* ScanController::getScanData(){
    return SD_;
}
