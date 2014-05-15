#include "recordkeeper.h"
#include <QDateTime>
#include <QSettings>
#include <QFile>
#include <QTextStream>

recordkeeper::recordkeeper(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    int updateTime = settings.value("time/update").toInt();
    if (updateTime == 0){
        updateTime = 10000;
        settings.setValue("time/update",updateTime);
    }
    timer_->setInterval(updateTime);
    connect(timer_,SIGNAL(timeout()),this,SLOT(updateTime()));

    xmlrecord_ = QDomDocument("times");


    // If the file exists load it into XML records
    QFile file(settings.value("time/file","records.xml").toString());
    bool loaded = true;
    if (file.open(QIODevice::ReadOnly)){
        if (!xmlrecord_.setContent(&file)) {
            file.close();
            loaded = false;
        }
    }else{
            loaded = false;
            file.close();
    }
    // if it doesnt exist, make a new record
    if(!loaded){
        QDomElement root = xmlrecord_.createElement("times");
        xmlrecord_.appendChild(root);
    }

    appendTime(kOff,settings.value("currentTime",QDateTime::currentMSecsSinceEpoch()).toInt());
}

void recordkeeper::appendTime(kTimeType type, int timeInMSSinceEpoch){
    QDomElement time = xmlrecord_.createElement("time");
    if(kOn==type){
        time.setAttribute("type","on");
    }else if (kOff==type){
        time.setAttribute("type","off");
    }else if (kScan==type){
        time.setAttribute("type","scan");
    }
    QDomText txt= xmlrecord_.createTextNode(QString::number(timeInMSSinceEpoch));
    time.appendChild(txt);
    xmlrecord_.documentElement().appendChild(time);

    QSettings settings;
    QFile file(settings.value("time/file","records.xml").toString());
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    QTextStream ss(&file);
    ss<<xmlrecord_.toString();
    file.close();

}


QDomDocument recordkeeper::getAndClearRecords(){
    QDomDocument returndoc;
    returndoc.setContent(xmlrecord_.toString());
    xmlrecord_.clear();
    QSettings settings;
    QFile file(settings.value("time/file","records.xml").toString());
    if(!file.open(QIODevice::WriteOnly)){
        return returndoc;
    }
    QTextStream ss(&file);
    ss<<xmlrecord_.toString();
    file.close();
    return returndoc;

}

void recordkeeper::scanStarted(){
    appendTime(kScan,QDateTime::currentMSecsSinceEpoch());
}

void recordkeeper::updateTime(){
    QSettings settings;
    settings.setValue("currentTime",QDateTime::currentMSecsSinceEpoch());
}

/**
<times>
    <time type="off">123124003</time>
    <time type="on">1231241313</time>
    <time type="scan">123124133</time>
</times>
**/
