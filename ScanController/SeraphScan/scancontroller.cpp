#include "scancontroller.h"
#include <QDomDocument>
#include <QDebug>

ScanController::ScanController(QObject *parent):QObject(parent),camNumber_(-1),
    axis_(""),ready_(0),scandistance_(0),
    stepsize_(0),scanspeed_(0),position_(0),xvector_(QVector<float>(3,0))
{    
    timer_ = new QTimer();
    connect(timer_,SIGNAL(timeout()),this,SLOT(ScanStep()));
    connect(this,SIGNAL(scanComplete()),timer_,SLOT(stop()));
    axes_["x"]=0;
    axes_["y"]=1;
    axes_["z"]=2;
}

void ScanController::loadVM(VirtualPrinter* vm)
{
    vm_ = vm;
    isReady();
}

void ScanController::setCamera(int c)
{
    camNumber_ = c;

    capwebcam.open(c);

    if (capwebcam.isOpened()==false){
        qDebug()<<"Error: capwebcam not accessed succesfully";
        return;
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
    if((capwebcam.isOpened()==false)||(axis_=="")||(scandistance_==0)||(stepsize_==0)||(scanspeed_==0)){//||(vm_==NULL)
        ready_=false;
        qDebug()<<QString("Isnt ready Webcam:") +QString::number((int)capwebcam.isOpened())+
                  QString(", Axis:")+QString(axis_)+QString(", ScanDistance:")+
                  QString::number(scandistance_)+QString(", StepSize:")+
                  QString::number(stepsize_)+QString(", scanspeed:")+QString::number(scanspeed_);
    }else{
        ready_=true;
        qDebug()<<"ready";
    }

    return ready_;
}


void ScanController::setScan(float scandistance, float stepsize, float scanspeed)
{
    scandistance_=scandistance;
    stepsize_=stepsize;
    scanspeed_=scanspeed;
    if (!((stepsize==0) || (scanspeed==0)))
    {
        timer_->setInterval((int)(stepsize/scanspeed)+1);
        //qDebug()<<QString("interval:")+ QString::number((int)(stepsize/scanspeed+1));
    }

    isReady();
}

void ScanController::StartScan()
{
    if(ready_){
        SD_ = new ScanData();
        timer_->start();
        emit scanRunning(true);
    }
}

void ScanController::StopScan()
{
    timer_->stop();
    emit scanRunning(false);
}

void ScanController::clearState(){
    ready_=false;
    scandistance_=0;
    stepsize_=0;
    scanspeed_=0;
    position_=0;

}


void ScanController::ScanStep()
{

    xvector_[0]=0;
    xvector_[1]=0;
    xvector_[2]=0;
    xvector_[axes_[axis_]]=stepsize_;
    position_+=stepsize_;


    if((ready_)&&(position_<scandistance_)){
        qDebug()<<"Start Move";
       // vm_->move(xvector_[0],xvector_[1],xvector_[2],scanspeed_);

        vm_->eInterface.getMotor(0)->moveRelative(1,1,0.1);
        ///CAPTURE DATA////
        capwebcam.read(matOriginal);
        if(matOriginal.empty()==true){
            qDebug()<<"ERROR READING WEBCAM";
        }
        QImage qimgOriginal((uchar*)matOriginal.data,matOriginal.cols,matOriginal.rows,matOriginal.step,QImage::Format_RGB888);
        SD_->addImage(position_,qimgOriginal);

        emit update(position_);
        emit updateStatus(QString("Position is now ")+QString::number(position_));

    }else{
        qDebug()<<QString("DONE");
        emit scanComplete();
        clearState();
    }
}

ScanData* ScanController::getScanData(){
    return SD_;
}
