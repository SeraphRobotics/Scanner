#include <QCoreApplication>

#include "scanprocessing.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ScanProcessing sp;

    qDebug()<<"files:\n"<<sp.filenames_;

    sp.processScan();

    for(int i=0; i<sp.pointCloud.size();i++){
        FAHVector3 v = sp.pointCloud[i];
        qDebug()<<"(" <<v.x<<","<<v.y <<","<<v.z <<")";
    }




    return a.exec();
}
