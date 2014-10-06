#include <QCoreApplication>
#include "scanmerger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    scanmerger();

    return a.exec();
}
