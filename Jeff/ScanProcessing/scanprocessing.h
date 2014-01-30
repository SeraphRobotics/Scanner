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
#include <QMap>
#include <QVector>
#include "xygrid.h"


class ScanProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ScanProcessing(QObject *parent = 0);

signals:

public slots:
    void setDir(QString dir);
    void processScan();
    XYGrid<float>* makeGrid();
    QString cloudCSV();

public:
    QMap<float, QVector < FAHVector3 >* > pointCloud;

//private:
    QStringList filenames_;
    QString extension_;
    QDir dir_;
};




#endif // SCANPROCESSING_H
