#include "recordkeeper.h"
#include <QDateTime>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDebug>

recordkeeper::recordkeeper(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    int updateTime = settings.value("time/update").toInt();
    if (updateTime == 0){
        updateTime = 10000;
        settings.setValue("time/update",updateTime);
    }
    timer_ = new QTimer();
    timer_->setInterval(updateTime);
    connect(timer_,SIGNAL(timeout()),this,SLOT(updateTime()));
    timer_->start();
    xmlrecord_ = QDomDocument("times");

    appendTime(kOff,settings.value("time/currentTime",QDateTime::currentMSecsSinceEpoch()).toLongLong());
    appendTime(kOn, QDateTime::currentMSecsSinceEpoch());
    qDebug()<<"Time "<<QDateTime::currentMSecsSinceEpoch();
}

void recordkeeper::appendTime(kTimeType type, qint64 timeInMSSinceEpoch){

    qDebug()<<"update:"<<type<<" at "<<timeInMSSinceEpoch;

    //////////// Open the File////////////////
    // If the file exists load it into XML records
    QSettings settings;
    QFile file_r(settings.value("time/file","records.xml").toString());
    bool loaded = true;
    if (file_r.open(QIODevice::ReadOnly)){
        if (!xmlrecord_.setContent(&file_r)) {
            file_r.close();
            loaded = false;
        }
    }else{
            loaded = false;
            file_r.close();
    }
    // if it doesnt exist, make a new record
    if(!loaded){
        QDomElement root = xmlrecord_.createElement("times");
        xmlrecord_.appendChild(root);
    }



    ///////////////////////Create entry//////////////////////////
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



    ///Write updated time
    QFile file(settings.value("time/file","records.xml").toString());
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    QTextStream ss(&file);
    ss<<xmlrecord_.toString();
    file.close();

}

void recordkeeper::updateTime(){
    QSettings settings;
    qDebug()<<"called";
    settings.setValue("time/currentTime",QDateTime::currentMSecsSinceEpoch());
}

/**
<times>
    <time type="off">123124003</time>
    <time type="on">1231241313</time>
    <time type="scan">123124133</time>
</times>
**/
