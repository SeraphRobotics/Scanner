#include <QCoreApplication>
#include <QSettings>
#include "recordkeeper.h"
#include "usbminder.h"
#include "scannerarduinointerface.h"

#include "../qdevicewatcher-master/src/qdevicewatcher.h"
#include "../qdevicewatcher-master/src/qdevicechangeevent.h"

#include "scannerwatcher.h"
#include "scannercontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("ScannerMinder");
    QSettings::setDefaultFormat(QSettings::IniFormat);

//    recordkeeper rk;
    //USBMinder* um = new USBMinder();
//    rk.connect(&um,SIGNAL(usbadded(QString)),&rk,SLOT(USBAdded(QString)));

//    ScannerArduinoInterface sai("COM5",BAUD9600);
//    QTimer::singleShot(2000,&sai,SLOT(laserOn()));
    ScannerWatcher* sw = new ScannerWatcher();
    ScannerController* sc = new ScannerController();
    QTimer::singleShot(2000,sc,SLOT(buttonPress()));

    sc->connect(sw,SIGNAL(scannerPort(QString)),sc,SLOT(portSelected(QString)));

    return a.exec();
}
