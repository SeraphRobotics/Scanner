#ifndef MASTERCONTROLUNIT_H
#define MASTERCONTROLUNIT_H

#include <QObject>
#include "mainwindow.h"
#include <QDomDocument>
#include "scancontroller.h"
#include <QTimer>


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
    void scanState(bool b);


private slots:
    void updateDebug();

public:
    QString debug;


private:
    MainWindow* MW_;
    ScanController* SC_;
    VirtualPrinter* VM_;
    QTimer* timer_;
};

#endif // MASTERCONTROLUNIT_H
