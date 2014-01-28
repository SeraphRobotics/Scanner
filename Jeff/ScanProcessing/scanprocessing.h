#ifndef SCANPROCESSING_H
#define SCANPROCESSING_H

#include <QObject>
#include <QDir>
#include <QVector>

#include <float.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "shared/fabathome-constants.h"

//struct FAHVector3{
//    float x;
//    float y;
//    float z;
//};

//QDebug operator<<(QDebug dbg,FAHVector3 v );

class ScanProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ScanProcessing(QObject *parent = 0);

signals:

public slots:
    void processScan();

public:
    QVector < FAHVector3 > pointCloud;

//private:
    QStringList filenames_;
    QDir dir_;
};

#endif // SCANPROCESSING_H
