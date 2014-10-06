#include "scanmerger.h"
#include <QDir>
#include <QDebug>
#include <vector>

void scanmerger(){
    QString s = "scan1";

    QDir d(s);
    qDebug()<<"Folder: "<<d.absolutePath();
    QStringList filters;
    filters<<"*.jpeg";
    QStringList files = d.entryList(filters);

    qDebug()<<"Files: \n"<<files;
    qDebug()<<"Filter:"<<filters;
    QString first = s+"//"+files.takeFirst();
    cv::Mat baseImg = cv::imread( first.toStdString() );

    foreach(QString filename, files){
        QString file = s+"//"+filename;
        cv::Mat nextimg = cv::imread(file.toStdString() );
        baseImg = baseImg+nextimg;
    }

    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(99);
    cv::imwrite("summed.jpeg",baseImg,params);
}

void scanpurge(){
    QString indir = "scan3";
    QString outdir = "cleanscan";


    QDir d(indir);
    qDebug()<<"Folder: "<<d.absolutePath();
    QStringList filters;
    filters<<"*.jpeg";
    QStringList infiles = d.entryList(filters);
//    qDebug()<<"Files: \n"<<infiles;
//    qDebug()<<"Filter:"<<filters;


    // Load offest
    QString toSubtract = "summed.jpeg";
    cv::Mat baseImg = cv::imread( toSubtract.toStdString() );

    foreach(QString filename, infiles){
        QString file = indir+"//"+filename;
        qDebug()<<file;
        cv::Mat nextimg = cv::imread(file.toStdString() );
        nextimg = nextimg-baseImg;

        std::vector<int> params;
        params.push_back(CV_IMWRITE_JPEG_QUALITY);
        params.push_back(99);
        QString writename = outdir+"//"+filename;
        cv::imwrite(writename.toStdString(),nextimg,params);
    }


}
