#include "scannerarduinointerface.h"
#include <QDebug>

ScannerArduinoInterface::ScannerArduinoInterface(QString port, BaudRateType baudrate, QObject *parent) :
    QObject(parent)
{
    port_ = new QextSerialPort(port);
    port_->setBaudRate(baudrate);
    timer_ = new QTimer();
    timer_->setInterval(100);
    connect(timer_,SIGNAL(timeout()),this,SLOT(_checkBuffer()));
}


bool ScannerArduinoInterface::connectPort(QString port, BaudRateType baudrate){
    if (port_->isOpen()){port_->close();}
    delete port_;
    port_ = new QextSerialPort(port);
    port_->setBaudRate(baudrate);
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
    port_->write(s.toStdString().c_str(),s.size());
}

void ScannerArduinoInterface::_checkBuffer(){
    if (port_->isOpen()){
        if(port_->bytesAvailable()>1){
            char char_in = port_->read(&char_in,1);
            QString c = QChar(char_in);
            if ("B"==c){
                emit buttonPressed();
            }else if ("D"==c){
                emit scanMovementCompleted();
            }else if ("E"==c){
                emit errored();
            }else{
                qDebug()<<"received: "+c;
            }
            qDebug()<<"received: "+c;
        }
    }
}
