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
    void setCamera(int c);
    void setAxis(QString Axis);
    bool isReady();
    void setScan(float stepdistance, float stepsize, float scanspeed);

public slots:
    void StartScan();
    void StopScan();
    void ScanStep();

signals:
    void update(float position);
    void updateStatus(QString s);
    void scanRunning(bool s);
    void scanComplete();

private:
    void clearState();


private:
    VirtualPrinter* vm_;
    ScanData* SD_;
    int camNumber_;
    QString axis_;
    bool ready_;
    float scandistance_;
    float stepsize_;
    float scanspeed_;
    float position_;
    QTimer* timer_;
    QMap<QString,int> axes_;
    QVector<float> xvector_;
    cv::VideoCapture capwebcam;
    cv::Mat matOriginal;


};

#endif // SCANCONTROLLER_H
