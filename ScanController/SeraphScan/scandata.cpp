#include "scandata.h"
#include <QSettings>

ScanData::ScanData(QObject *parent) :
    QObject(parent),extension_("JPEG")
{

    id_ = QUuid::createUuid();
    QSettings settings;
    folderLocation_ = QDir(QFileInfo(settings.fileName()).absolutePath()+QDir::separator()+id_.toString());
}



void ScanData::addImage(float x, QPixmap img)
{
    image_map_[x]=img;

}



QPixmap ScanData::getImageFromX(float x)
{

    if(image_map_.keys().contains(x)){
        return image_map_[x];
    }else{
        return QPixmap();
    }

}
QList<float> ScanData::getXRange()
{
    return image_map_.keys();
}

void ScanData::writeToDisk(){
    qDebug()<<"\ncalled";

    if (!folderLocation_.exists()){
        folderLocation_.mkdir(folderLocation_.absolutePath());
    }
    qDebug()<<QString("folderLocation ")+QString(folderLocation_.exists() ? "exists" : "doesnt exist");

    bool worked=true;
    QList<float> keys = getXRange();
    for(int i=0;i<keys.size();i++){
        float x = keys.at(i);
        QString s = folderLocation_.absolutePath()+QDir::separator()+QString::number(x)+QString(".")+extension_;
        bool saved = false;
        int counter = 0;
        while(!saved && (counter<1000)){
            saved = image_map_[x].save(s);
            counter++;
        }
        worked = worked && saved;
        qDebug()<<"image: "<<i<<"\tposition: "<<x;


    }

    qDebug()<<QString(worked? "Worked" : "Didnt Work");

}
