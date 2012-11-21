#ifndef MASTERCONTROLUNIT_H
#define MASTERCONTROLUNIT_H

#include <QObject>
#include "mainwindow.h"
#include <QDomDocument>
#include"scancontroller.h"


class MasterControlUnit : public QObject
{
    Q_OBJECT
public:
    explicit MasterControlUnit(QObject *parent = 0);
    void loadObjects(MainWindow* MW,ScanController* SC);
    void connectToVM(QString filestr, QString Port);
    void makeConnections();


signals:
    
public slots:
    void startScan();



private:
    MainWindow* MW_;
    ScanController* SC_;
    VirtualPrinter* VM_;
};

#endif // MASTERCONTROLUNIT_H
