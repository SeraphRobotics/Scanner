#include <QCoreApplication>

#include "scanprocessing.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ScanProcessing sp;

    qDebug()<<"files:\n"<<sp.filenames_;

    sp.processScan();

    for(int i=0; i<sp.pointCloud.keys().size();i++){
        float x = sp.pointCloud.keys()[i];
        int size = sp.pointCloud.value(x)->size();
        qDebug()<<"{"<<x<<","<<size<<"}";
//        qDebug()<<"(" <<v.x<<","<<v.y <<","<<v.z <<")";
    }


    sp.makeGrid();



    return a.exec();
}
