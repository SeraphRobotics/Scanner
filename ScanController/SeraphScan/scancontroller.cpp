#include "scancontroller.h"
#include <QDomDocument>
#include <QDebug>

ScanController::ScanController(QObject *parent):QObject(parent),camNumber_(-1),
    axis_(""),ready_(0),scandistance_(0),
    stepsize_(0),framerate_(0),position_(0),targetposition_(0),xvector_(QVector<float>(3,0))
{    
    timer_ = new QTimer();
    vm_ = new VirtualPrinter();
    SD_ = new ScanData();
    connect(timer_,SIGNAL(timeout()),this,SLOT(ScanStep()));
    connect(this,SIGNAL(scanComplete()),timer_,SLOT(stop()));

    // Used in motion commands durring scan
    axes_["x"]=0;
    axes_["y"]=1;
    axes_["z"]=2;

}

void ScanController::loadVM(VirtualPrinter* vm)
{
    delete vm_;
    vm_ = vm;

    isReady();
}

void ScanController::setCamera(int c)
{
    camNumber_ = c;

    capwebcam.open(c);

    if (capwebcam.isOpened()==false){
        emit error(QString("Error: capwebcam not accessed succesfully"));
        return;
    }

    //ERROR: THIS IS CONFIG SETTINGS< SHOULD BE IN A CONFIGURATION
    //check if camera set properly
    bool state=true;
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_WIDTH,1920);
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_HEIGHT,1080);

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
        xvector_[0]=0;
        xvector_[1]=0;
        xvector_[2]=0;
        xvector_[axes_[axis_]]=scandistance_;

        //vm_->xyzmotion->setAcceleration(100);
        float speed = framerate_*stepsize_;

        vm_->move(xvector_[0],xvector_[1],xvector_[2],speed);

        timer_->start();
        targetposition_+=0;//stepsize_;
        emit scanRunning(true);
    }else{
        emit error(QString("not ready"));
    }
}

void ScanController::StopScan()
{
    timer_->stop();
    //THIS IS A TEMP SOLUTION, WE NEED A NON-FORCED STOP
    vm_->stop();
    SD_->writeToDisk();
    emit scanRunning(false);
}

void ScanController::clearState(){
    ready_=false;
    scandistance_=0;
    stepsize_=0;
    framerate_=0;
    position_=0;

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

//    qDebug()<<"error: "<<error<<"\ttaget:"<<targetposition_;

    if(fabs(p_error)<tolerance){
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
//        clearState();
        emit scanComplete();
    }
}

ScanData* ScanController::getScanData(){
    return SD_;
}
