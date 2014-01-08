#include "scandata.h"
#include <QSettings>

ScanData::ScanData(QObject *parent) :
    QObject(parent),extension_(".jpeg")
{

    id_ = QUuid::createUuid();
    QSettings settings;
    folderLocation_ = QDir(QFileInfo(settings.fileName()).absolutePath()+QDir::separator()+id_.toString());


//    qDebug()<<"folderLocation is " + folderLocation_.absolutePath();

    if (!folderLocation_.exists()){
        folderLocation_.mkdir(folderLocation_.absolutePath());
    }
    qDebug()<<QString("folderLocation ")+QString(folderLocation_.exists() ? "exists" : "doesnt exist");
}

void ScanData::addImage(float x, QImage img)
{
    xs_.append(x);
    QString s = folderLocation_.absolutePath()+QDir::separator()+QString::number(x)+extension_;

    bool worked;
    worked = img.save(s);

//    qDebug()<<QString(worked? "Worked" : "Didnt Work")+ QString("\tLocation: ")+s;
}



QImage ScanData::getImageFromX(float x)
{
    const QString s = folderLocation_.absolutePath()+QDir::separator()+QString::number(x)+extension_;
    QImage img(s);
    return img;
}
QList<float> ScanData::getXRange()
{
    return QList<float>(xs_);
}

