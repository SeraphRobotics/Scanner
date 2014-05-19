#include <QCoreApplication>
#include <QSettings>
#include "recordkeeper.h"

#include "../qdevicewatcher-master/src/qdevicewatcher.h"
#include "../qdevicewatcher-master/src/qdevicechangeevent.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("ScannerMinder");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    recordkeeper rk;
    QDeviceWatcher watcher();

    return a.exec();
}
