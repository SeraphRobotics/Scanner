#include "scannercontroller.h"
#include <QPixmap>
#include <QDebug>
#include <QSettings>
#include <QUuid>
#include <QDir>
#include <QDomDocument>
#include <QDateTime>

ScannerController::ScannerController(QObject *parent) :
    QObject(parent),camNumber_(1),scandistance_(334),stepsize_(1),framerate_(5),width_(1920),height_(1080),dist(0),uuid_(".")
{
    QSettings s;
    camNumber_ = s.value("camNumber",1).toInt();
    sai_=  new ScannerArduinoInterface();
    timer_ = new QTimer();
    timer_->setInterval(1000.0/framerate_*0.925);
    connect(timer_,SIGNAL(timeout()),this,SLOT(ScanStep()));




}

void ScannerController::disconnected(){
    capwebcam.release();
    sai_->disconnect();
    delete sai_;
    sai_=  new ScannerArduinoInterface();
    timer_->stop();
}

void ScannerController::portSelected(QString port){
//    sai_->disconnect();
    delete sai_;
    sai_=  new ScannerArduinoInterface(port,BAUD9600);
    connect(sai_,SIGNAL(buttonPressed()),this,SLOT(buttonPress()));
    connect(sai_,SIGNAL(errored()),this,SLOT(scannerError()));
    connect(sai_,SIGNAL(scanMovementCompleted()),this,SLOT(scanComplete()));

    qDebug()<<"selected: "<<port<<" opened:"<<sai_->isReady();
//    QTimer::singleShot(3000,this,SLOT( setupCamera() ));
//    QTimer::singleShot(2000,sai_,SLOT(ledOn()));
}

void ScannerController::setupCamera(){
    capwebcam.open(camNumber_);
    bool state=true;
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_WIDTH,width_);
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_HEIGHT,height_);

    if(!state){
        qDebug()<< QString("Error setting capture sizes)");
    }

    if ( capwebcam.isOpened() ){
        qDebug()<<"opened camera";
    }else{
        qDebug()<<"is opened";
    }
}

void ScannerController::buttonPress(){
    if (!timer_->isActive() ){
        uuid_ = QUuid::createUuid().toString();
        QDir d;
        if(!d.mkdir(uuid_)){
            qDebug()<<"Could Not open UUID dir "<< uuid_;
            return;
        }

        sai_->startScan();
        timer_->start();
        dist=0;
        setupCamera();
    }
}
void ScannerController::scannerError(){}
void ScannerController::scanComplete(){
    QString filename = "ScanData.xml";
    QDomDocument d;
    QDomElement root;

    // if the file exists, read the file
    QDir dir;
    if(dir.exists(filename)){
        QFile usbdata(filename);

        if (!usbdata.open(QFile::ReadOnly)) {
            qDebug() << "Could not open ScanData file.";
        }

        if (!d.setContent(&usbdata)) {
            usbdata.close();
            qDebug()<<"Could not load Data from ScanData.xml";
            return;
        }
        usbdata.close();
        root = d.documentElement();

    }else{
        //If the file doesnt exist make a new root.
        root = d.createElement("ScanData");
    }

    QDomElement node = d.createElement("Scan");
    QDomElement Folder = d.createElement("Folder");
    Folder.appendChild(d.createTextNode(uuid_));
    node.appendChild(Folder);

    QDomElement datetime = d.createElement("DateTime");
    datetime.appendChild(d.createTextNode(QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())));
    node.appendChild(datetime);

    root.appendChild(node);
    d.appendChild(root);

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)){
        qDebug()<<"could not open file: "<< filename;
        return;
    }
    QTextStream f(&file);
    f<<d.toString();
    file.close();

}

void ScannerController::ScanStep(){
    if(!capwebcam.isOpened()){return;}
    capwebcam.read(matOriginal);


    dist += stepsize_;

    if(matOriginal.empty()==true){
        qDebug()<<QString("Error reading camera at position:")<<QString::number(dist);
    }

    cv::Mat dest;
    cv::cvtColor(matOriginal, dest,CV_BGR2RGB);

    QImage m = QImage((unsigned char*) dest.data,dest.cols,dest.rows,dest.step,QImage::Format_RGB888);
    QString writename = uuid_ + "//"+QString::number(dist)+".jpeg";
    m.save(writename,"JPEG");
    if (dist>scandistance_){
        timer_->stop();
        capwebcam.release();
        scanComplete();
    }

}
