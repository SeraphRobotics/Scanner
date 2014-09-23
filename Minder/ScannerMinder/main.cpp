#include <QCoreApplication>
#include <QSettings>
#include "recordkeeper.h"
#include "usbminder.h"
#include "scannerarduinointerface.h"

#include "../qdevicewatcher-master/src/qdevicewatcher.h"
#include "../qdevicewatcher-master/src/qdevicechangeevent.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("ScannerMinder");
    QSettings::setDefaultFormat(QSettings::IniFormat);

//    recordkeeper rk;
//    USBMinder um;
//    rk.connect(&um,SIGNAL(usbadded(QString)),&rk,SLOT(USBAdded(QString)));

    ScannerArduinoInterface sai("COM5",BAUD9600);
    QTimer::singleShot(0,&sai,SLOT(laserOn()));
    return a.exec();
}
