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
    isReady();
}

void ScanController::setAxis(QString Axis)
{
    axis_ = Axis.toLower();
    isReady();
}


bool ScanController::isReady()
{
    if((camNumber_<0)||(axis_=="")||(scandistance_==0)||(stepsize_==0)||(scanspeed_==0)){//||(vm_==NULL)
        ready_=false;
    }else{
        ready_=true;
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

 //       vm_->move(xvector_[0], xvector_[1], xvector_[2], scanspeed_);
        //qDebug()<<QString("Position is now ")+QString::number(position_);
        emit update(position_);
        emit updateStatus(QString("Position is now ")+QString::number(position_));
        ///CAPTURE DATA////
    }else{
        //qDebug()<<QString("DONE");
        emit scanComplete();
        clearState();
    }
}
