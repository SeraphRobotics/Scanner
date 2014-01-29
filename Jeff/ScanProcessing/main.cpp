#include <QCoreApplication>

#include "scanprocessing.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ScanProcessing sp;

    QString basedir = "C:\\Users\\Jeffrey\\Dropbox\\Seraph Documents\\scans\\";

    QStringList strings;
    strings.append("Box1-sample1-Left");
//    strings.append("Box1-sample2-Left");
//    strings.append("Box1-sample3-Left");
//    strings.append("Box1-sample4-Left");
//    strings.append("Box1-sample1-Right");
//    strings.append("Box1-sample2-Right");
//    strings.append("Box1-sample3-Right");
//    strings.append("Box1-sample4-Right");


    for(int k=0; k<strings.size();k++){
        QString dir  =basedir+strings.at(k);
        sp.setDir(dir);
        sp.processScan();
        XYGrid<float>* grid = sp.makeGrid();
        QFile f(strings.at(k)+".csv");
        if (!f.open(QFile::WriteOnly)) {
            printf("\nFAILED TO OPEN FILE\n");
            return false;
        }
        QTextStream out(&f);
//        grid->rotate90();
        out<<grid->toCSV();
    //    qDebug()<<grid->toCSV();
        f.close();
        qDebug()<<"Made: "<<strings.at(k);
    }



    return 0;
}
