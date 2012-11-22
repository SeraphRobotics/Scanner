#ifndef SCANDATA_H
#define SCANDATA_H

#include <QObject>
#include <QImage>
#include <QUuid>
#include <QtCore>

class ScanData : public QObject
{
    Q_OBJECT
public:
    explicit ScanData(QObject *parent = 0);
    void addImage(float x, QImage img);
    QImage getImageFromX(float x);
    QList<float> getXRange();

signals:
    
public slots:


private:
    QDir folderLocation_;
    QUuid id_;
    QString extension_;
    QList<float> xs_;

};

#endif // SCANDATA_H
