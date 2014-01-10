#ifndef MASTERCONTROLUNIT_H
#define MASTERCONTROLUNIT_H

#include <QObject>
#include <QDomDocument>
#include "scancontroller.h"
#include <QTimer>


class MasterControlUnit : public QObject
{
    Q_OBJECT
public:
    explicit MasterControlUnit(QObject *parent = 0);
    ~MasterControlUnit();
    void connectToVM(QString filestr, QString Port);

signals:
    void error(QString);
    void image(QPixmap);

public slots:
    void startScan();
    void scanState(bool b);


private slots:
    void updateDebug();
    void makeConnections();

public:
    QString debug;


private:
    ScanController* SC_;
    VirtualPrinter* VM_;
    QTimer* timer_;
};

#endif // MASTERCONTROLUNIT_H
