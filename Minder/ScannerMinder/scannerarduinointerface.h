#ifndef SCANNERARDUINOINTERFACE_H
#define SCANNERARDUINOINTERFACE_H

#include <QObject>
#include <QTimer>
#include "qextserialport.h"

class ScannerArduinoInterface : public QObject
{
    Q_OBJECT
public:
    explicit ScannerArduinoInterface(QString port, BaudRateType baudrate, QObject *parent = 0);

signals:
    void buttonPressed();
    void scanMovementCompleted();
    void errored();

public slots:
    bool connectPort(QString port, BaudRateType baudrate);
    bool isReady();
    void startScan();
    void laserOn();
    void laserOff();
    void ledOn();
    void ledOff();
    void home();

private slots:
    void _write(QString s);
    void _checkBuffer();

private:
    QTimer* timer_;
    QextSerialPort* port_;
};

#endif // SCANNERARDUINOINTERFACE_H
