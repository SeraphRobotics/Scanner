#ifndef SCANDATA_H
#define SCANDATA_H

#include <QObject>
#include <QPixmap>
#include <QUuid>
#include <QtCore>

class ScanData : public QObject
{
    Q_OBJECT
public:
    explicit ScanData(QObject *parent = 0);
    QPixmap getImageFromX(float x);
    QList<float> getXRange();

signals:
    
public slots:
    void addImage(float x, QPixmap img);
    void writeToDisk();


private:
    QDir folderLocation_;
    QUuid id_;
    QString extension_;
    QMap<float,QPixmap> image_map_;

};

#endif // SCANDATA_H
