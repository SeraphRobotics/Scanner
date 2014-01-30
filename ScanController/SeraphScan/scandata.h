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
    QList<float> getXRange();

signals:
    
public slots:
    void addImage(float x, QPixmap pxmap);
    XYGrid<float>* makeGrid();

public:
    QMap<float, QVector < FAHVector3 >* > pointCloud;



};



cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData);
cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData);


#endif // SCANDATA_H
