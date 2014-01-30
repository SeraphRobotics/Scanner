#ifndef SCANCONTROLLER_H
#define SCANCONTROLLER_H

#include "FabConLib/FabConLib/virtualmachines.h"
#include <QString>
#include <QObject>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "scandata.h"

class ScanController: public QObject
{
    Q_OBJECT
public:
    explicit ScanController(QObject *parent = 0);
    void loadVM(VirtualPrinter* vm);
    void loadScanData(ScanData* sd);
    bool isReady();
    ScanData* getScanData();

public slots:
    void StartScan();
    void StopScan();
    void setScan(float scandistance, float stepsize, float framerate);
    void setCamera(int c);
    void setWidth(int w);
    void setHeight(int h);
    void setAxis(QString Axis);

private slots:
    void ScanStep();

signals:
    void update(float position);
    void updateStatus(QString s);
    void scanRunning(bool s);
    void scanComplete();
    void error(QString s);
    void image(float x, QPixmap px);



private:
    VirtualPrinter* vm_;
    ScanData* SD_;
    QTimer* timer_;
    int camNumber_;
    QString axis_;
    bool ready_;
    float scandistance_;
    float stepsize_;
    float framerate_;
    float position_;
    float targetposition_;
    QMap<QString,int> axes_;
    QVector<float> xvector_;
    cv::VideoCapture capwebcam;
    cv::Mat matOriginal;
    int width_;
    int height_;


};

#endif // SCANCONTROLLER_H
