#include "scannerarduinointerface.h"
#include <QDebug>

ScannerArduinoInterface::ScannerArduinoInterface(QString port, BaudRateType baudrate, QObject *parent) :
    QObject(parent)
{
    port_ = new QextSerialPort(port);//,QextSerialPort::Polling
    port_->setBaudRate(BAUD9600);
    port_->setFlowControl(FLOW_OFF);
    port_->setParity(PAR_NONE);
    port_->open(QIODevice::ReadWrite);
    //    port_->set
    timer_ = new QTimer();
    timer_->setInterval(100);

    connect(timer_,SIGNAL(timeout()),this,SLOT(_checkBuffer()));
//    timer_->start();
    connect(port_, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}


bool ScannerArduinoInterface::connectPort(QString port, BaudRateType baudrate){
    if (port_->isOpen()){port_->close();}
    delete port_;
    port_ = new QextSerialPort(port);
    port_->setBaudRate(BAUD9600);
    port_->setFlowControl(FLOW_OFF);
    port_->setParity(PAR_NONE);
    return port_->isOpen();
}

bool ScannerArduinoInterface::isReady(){
    return port_->isOpen();
}
void ScannerArduinoInterface::startScan(){
    _write("s");
}
void ScannerArduinoInterface::laserOn(){
    _write("l");
}
void ScannerArduinoInterface::laserOff(){
    _write("k");
}
void ScannerArduinoInterface::ledOn(){
    _write("o");
}
void ScannerArduinoInterface::ledOff(){
    _write("f");
}
void ScannerArduinoInterface::home(){
    _write("h");
}


void ScannerArduinoInterface::_write(QString s){

    if (!isReady() ){return;}
    qDebug()<<"writing: "<<s.toStdString().c_str();

    QChar c = s.at(0);
//    QChar c(100);
    char fill= c.cell();
    QByteArray ba(1,fill);
//    ba.push_front(c.row());
    qint64 l = ba.length();
    qDebug()<<"writing: "<<ba.toHex();
    qint64 received  = port_->write(ba,l);
    qDebug()<<"wrote:"<<received;
}

void ScannerArduinoInterface::_checkBuffer(){
    if (port_->isOpen()){
        if(port_->bytesAvailable()>0){
            char char_in;
            port_->read( &char_in,1);
//            qDebug()<<"read: "<<readbytes;
            QString c = QChar(char_in);
            if ("B"==c){
                emit buttonPressed();
            }else if ("D"==c){
                emit scanMovementCompleted();
            }else if ("E"==c){
                emit errored();
            }else{
                //qDebug()<<"received: "+c;
            }
            qDebug()<<"received: "+char_in;
        }
    }
}

void ScannerArduinoInterface::onDataAvailable(){
    QByteArray data = port_->readAll();
    qDebug()<<"received: "<<QString(data);
    QString c = QString(data);
    if ("B"==c){
        emit buttonPressed();
    }else if ("D"==c){
        emit scanMovementCompleted();
    }else if ("E"==c){
        emit errored();
    }else{
        qDebug()<<"received: "+c;
    }
}
