#include "mastercontrolunit.h"

MasterControlUnit::MasterControlUnit(QObject *parent) :
    QObject(parent)
{
    VM_ = new VirtualPrinter();
}

void MasterControlUnit::loadObjects(MainWindow* MW,ScanController* SC){
    MW_=MW;
    SC_=SC;
    SC_->setScan(100,1,1);
    SC_->setCamera(0);
    SC_->setAxis("x");
    makeConnections();
}


void MasterControlUnit::startScan(){
    if (!(SC_->isReady())){
        SC_->setScan(100,1,1);
        SC_->StartScan();
    }
}

void MasterControlUnit::connectToVM(QString filestr, QString port){
//    VM_.setComPort(port);
//    QDomDocument document;
//    document.setContent(filestr);
//    VM_.loadConfig(document);
    SC_->loadVM(VM_);



}
void MasterControlUnit::makeConnections(){
    connect(MW_,SIGNAL(start()),SC_,SLOT(StartScan()));
    connect(MW_,SIGNAL(stop()),SC_,SLOT(StopScan()));
    connect(SC_,SIGNAL(updateStatus(QString)),MW_,SLOT(appendText(QString)));
    connect(SC_,SIGNAL(scanComplete()),MW_,SLOT(on_startStopButton_clicked()));
    connect(SC_,SIGNAL(scanRunning(bool)),MW_,SLOT(setScanState(bool)));
    connect(MW_,SIGNAL(start()),this,SLOT(startScan()));

}
