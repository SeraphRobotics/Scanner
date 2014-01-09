#include "mastercontrolunit.h"
#include <QtAlgorithms>
#include <QDebug>

MasterControlUnit::MasterControlUnit(QObject *parent) :
    QObject(parent)
{
    VM_ = new VirtualPrinter();
    timer_ = new QTimer();
    timer_->setInterval(100);
    SC_ = new ScanController();
    SC_->setCamera(1);
    SC_->setAxis("x");
}

void MasterControlUnit::loadObjects(MainWindow* MW){
    //This should simply load the important objects and start the process.
    MW_=MW;
    makeConnections();
}


void MasterControlUnit::startScan(){
    if (!(SC_->isReady())){
        ///ERROR, TODO: CONFIGURATION INFO, SHOULD NOT BE HERE
        SC_->setScan(300,1,10);
        SC_->StartScan();
    }
}

void MasterControlUnit::connectToVM(QString filestr, QString port){
    VM_->setComPort(port);

    // Open file at filestr and use as config for VM
    QString db;
    QTextStream ss(&debug);
    QDomDocument document;
    QFile configFile(filestr);
    if (!configFile.open(QFile::ReadOnly)) {
        // Failed to open config file and update debugging
        db="Failed to open config file.";
        qDebug() <<db;
        ss<<db;
        return;
    }


    db="Opened Config File";
    qDebug()<<db;
    ss<<db;

    //Load the config to the VM_, vm connects and the vm is loaed to the scan controller
    document.setContent(&configFile);
    configFile.close();
    VM_->loadConfig(document);
    SC_->loadVM(VM_);



}
void MasterControlUnit::makeConnections(){
    connect(MW_,SIGNAL(start()),SC_,SLOT(StartScan()));
    connect(MW_,SIGNAL(stop()),SC_,SLOT(StopScan()));
    connect(SC_,SIGNAL(updateStatus(QString)),MW_,SLOT(appendText(QString)));

    //connect(SC_,SIGNAL(scanComplete()),MW_,SLOT(on_startStopButton_clicked()));

    connect(SC_,SIGNAL(scanRunning(bool)),MW_,SLOT(setScanState(bool)));
    connect(MW_,SIGNAL(start()),this,SLOT(startScan()));
    connect(SC_,SIGNAL(scanRunning(bool)),this,SLOT(scanState(bool)));
    connect(timer_,SIGNAL(timeout()),this,SLOT(updateDebug()));
    timer_->start();
}


void MasterControlUnit::scanState(bool b){

    //When Done scanning, post the last image to the Main Window.
    if(!b){
      ScanData* sd = SC_->getScanData();
      QList<float> l = sd->getXRange();
      if(!l.isEmpty()){
        qSort(l.begin(),l.end());
        float lastx = l.last();
        QPixmap img = sd->getImageFromX(lastx);
        MW_->setImage(img);
      }
    }
}

void MasterControlUnit::updateDebug(){
    QString debug_update = debug+SC_->debug;
    if(!debug_update.isEmpty()){
        MW_->appendText(debug_update);
        debug.clear();
        SC_->debug.clear();
    }
}
