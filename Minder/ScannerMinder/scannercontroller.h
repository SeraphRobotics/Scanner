#ifndef SCANNERCONTROLLER_H
#define SCANNERCONTROLLER_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "scannerarduinointerface.h"


class ScannerController : public QObject
{
    Q_OBJECT
public:
    explicit ScannerController(QObject *parent = 0);

signals:

public slots:
    void portSelected(QString port);


private slots:
    void buttonPress();
    void scannerError();
    void scanComplete();

    void ScanStep();

private:
    int camNumber_;
    float scandistance_;
    float stepsize_;
    float framerate_;
    int width_;
    int height_;
    float dist;
    QTimer* timer_;
    ScannerArduinoInterface* sai_;
    cv::VideoCapture capwebcam;
    cv::Mat matOriginal;




};

#endif // SCANNERCONTROLLER_H
