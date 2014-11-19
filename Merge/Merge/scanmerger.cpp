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
    int numsize = files.size()+1;

    cv::Mat baseImg = cv::imread( first.toStdString() );

//    int numsize = files.size()+1;


    int i=1;
    foreach(QString filename, files){
        QString file = s+"//"+filename;
        cv::Mat nextimg = cv::imread(file.toStdString() );
        baseImg = baseImg+nextimg;
        i++;
        if (i>35){break;}
//        cv::addWeighted(baseImg,alpha,nextimg,beta,0.0,baseImg);
    }

//    baseImg = baseImg/numsize;
    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(99);
    cv::imwrite("summed2.jpeg",baseImg,params);
    qDebug()<<"Done";
}

void scanpurge(){
    QString indir = "Correct_Foot";
    QString outdir = "cleanscan";


    QDir d(indir);
    qDebug()<<"Folder: "<<d.absolutePath();
    QStringList filters;
    filters<<"*.jpeg";
    QStringList infiles = d.entryList(filters);
//    qDebug()<<"Files: \n"<<infiles;
//    qDebug()<<"Filter:"<<filters;


    // Load offest
    QString toSubtract = "summed2.jpeg";
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
