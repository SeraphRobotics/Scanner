#include "mastercontrolunit.h"
#include <QtAlgorithms>
#include <QDebug>

MasterControlUnit::MasterControlUnit(QObject *parent) :
    QObject(parent)
{
    VM_ = new VirtualPrinter();
    SC_ = new ScanController();
    makeConnections();
}

MasterControlUnit::~MasterControlUnit(){
    delete VM_;
    delete SC_;
}

void MasterControlUnit::startScan(){
//    if (SC_->isReady()){
//        SC_->StartScan();
//    }else{
//        QString status = QString("Scan ready: ")+QString(SC_->isReady()?"true":"false");
//        emit error(status);
//    }

    VM_->move(100,0,0,10);
    QTimer::singleShot(2000,this,SLOT(test()));
}

void MasterControlUnit::test(){
    VM_->stop();
    QTimer::singleShot(100,this,SLOT(test2()));
}

void MasterControlUnit::test2(){
    QVector<double> p=VM_->currentPosition();
    qDebug()<<"Position: "<<p;
    VM_->move(-p[0],-p[1],-p[2],10);
}

void MasterControlUnit::loadScanConfig(QString filestr){
    QString db;
    QDomDocument document;
    QFile configFile(filestr);
    if (!configFile.open(QFile::ReadOnly)) {
        db="Failed to open config file.";
        qDebug() <<db;
        emit error(db);
        return;
    }


    db="Opened Config File";
    qDebug()<<db;
    emit update(db);

    document.setContent(&configFile);
    configFile.close();

    QDomElement root = document.documentElement();
    QDomNode scan = root.namedItem("scanner");
    QDomNode printer = root.namedItem("printer");


    //LOAD SCANNER SETTINGS
    if (!("scanner" == scan.nodeName().toLower())) {
        db = QString("\n Bad file passed, no scan settings found, found: ")+scan.nodeName();
        qDebug() << db;
        emit error(db);
        return;
    }

    if (!scan.hasChildNodes()) {
        db = QString("\n No settings in scan");
        emit error(db);
        return;
    }


    QString axis("");
    int camera =0;
    float length=0;
    float resolution=0;
    float framerate=0;

    QDomNodeList scanChildren = scan.childNodes();
    for (unsigned int i = 0; i < scanChildren.length(); i++) {
        QDomNode schild = scanChildren.at(i);
        QDomElement el;
        QString name = schild.nodeName().toLower();
        if (!schild.isElement()) {
            continue;
        }
        if ("camera"==name) {
            el=schild.toElement();
            camera = el.text().toUInt();
        }else if ("axis"==name) {
            el=schild.toElement();
            axis = el.text();
        }else if ("length"==name) {
            el=schild.toElement();
            length = el.text().toFloat();
        }else if ("resolution"==name) {
            el=schild.toElement();
            resolution = el.text().toFloat();
        }else if ("framerate"==name) {
            el=schild.toElement();
            framerate = el.text().toFloat();
        }
    }
    if((0==camera)||(0.1==length)||(0.01>resolution)||(0.1>framerate)||axis.isEmpty()){
        emit error("Settings invalid");
        return;
    }

    SC_->setScan(length,resolution,framerate);
    SC_->setCamera(camera);
    SC_->setAxis(axis);


    //LOAD PRINTER SETTINGS
    if (!("printer" == printer.nodeName().toLower())) {
        db = QString("\n Bad file passed, no printer settings found, found: ")+scan.nodeName();
        qDebug() << db;
        emit error(db);
        return;
    }

    if (!printer.hasChildNodes()) {
        db = QString("\n No settings in scan");
        emit error(db);
        return;
    }


    QString comport("");
    QString printerConfigFile("");
    QDomNodeList printerChildren = printer.childNodes();
    for (unsigned int i = 0; i < printerChildren.length(); i++) {
        QDomNode pchild = printerChildren.at(i);
        QDomElement el;
        QString name = pchild.nodeName().toLower();
        if (!pchild.isElement()) {
            continue;
        }
        if ("comport"==name) {
            el=pchild.toElement();
            comport = el.text();
        }else if ("config"==name){
            el=pchild.toElement();
            printerConfigFile = el.text();
        }
    }
    if(!(comport.isEmpty()||printerConfigFile.isEmpty())){
        connectToVM(printerConfigFile,comport);
    }else{
        QString message = QString("ERROR: comport or config file not loaded\n\tComport: ")+comport+QString("\n\tConfig: ")+printerConfigFile;
        emit error(message);
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
        emit error(db);
        return;
    }


    db="Opened Config File";
    qDebug()<<db;
    emit update(db);

    //Load the config to the VM_, vm connects and the vm is loaed to the scan controller
    document.setContent(&configFile);
    configFile.close();
    VM_->loadConfig(document);
    SC_->loadVM(VM_);



}

void MasterControlUnit::makeConnections(){

    connect(this->parent(),SIGNAL(start()),SC_,SLOT(StartScan()));
    connect(this->parent(),SIGNAL(stop()),SC_,SLOT(StopScan()));
    connect(this->parent(),SIGNAL(start()),this,SLOT(startScan()));

    connect(SC_,SIGNAL(scanRunning(bool)),this->parent(),SLOT(setScanState(bool)));
    connect(SC_,SIGNAL(updateStatus(QString)),this->parent(),SLOT(appendText(QString)));
    connect(SC_,SIGNAL(scanRunning(bool)),this,SLOT(scanState(bool)));
    connect(SC_,SIGNAL(error(QString)),this->parent(),SLOT(appenndText(QString)));
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
        emit image(img);
      }
    }
}

