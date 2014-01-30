#ifndef SCANDATA_H
#define SCANDATA_H

#include <QObject>
#include <QPixmap>
#include <QUuid>
#include <QtCore>
#include <float.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QMap>
#include <QVector>
#include "xygrid.h"


class ScanData : public QObject
{
    Q_OBJECT
public:
    explicit ScanData(QObject *parent = 0);
    ~ScanData();
    QList<float> getXRange();

signals:
    
public slots:
    void addImage(float x, QPixmap pxmap);
    XYGrid<float>* makeGrid();
    void processedImage(float x, QVector < FAHVector3 >* row);

public:
    QMap<float, QVector < FAHVector3 >* > pointCloud;



};






#endif // SCANDATA_H
