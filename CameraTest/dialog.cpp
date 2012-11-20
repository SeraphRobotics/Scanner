#include "dialog.h"
#include "ui_dialog.h"
#include <QtCore>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    capwebcam.open(1);

    if (capwebcam.isOpened()==false){
        ui->txtXYRadius->appendPlainText("Error: capwebcam not accessed succesfully");
        return;
    }

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()),this,SLOT(processFrameAndUpdateGUI()));
    tmrTimer->start(20);




}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnPauseOrResume_clicked()
{
    if(tmrTimer->isActive()){
        tmrTimer->stop();
        ui->btnPauseOrResume->setText("Resume");
    }else{
        tmrTimer->start();
        ui->btnPauseOrResume->setText("Pause");
    }
}

void Dialog::processFrameAndUpdateGUI(){
    capwebcam.read(matOriginal);
    if(matOriginal.empty()==true){return;}

    cv::inRange(matOriginal,				// funcion input
                cv::Scalar(125,0,0),		// min filtering value (if greater than or equal to this) (in BGR format)
                cv::Scalar(256,100,100),	// max filtering value (and if less than this) (in BGR format)
                matProcessed);				// function output

                                        // smooth the processed image, this will make it easier for the next function to identify circles
    cv::GaussianBlur(matProcessed,			// function input
                     matProcessed,			// function output
                     cv::Size(9,9),			// smoothing window width and height in pixels
                     1.5);					// sigma value, determines how much the image will be blurred

                                                // fill circles vector with all circles in processed image
    cv::HoughCircles(matProcessed,				// input image
                     vecCircles,				// function output (must be a standard template library vector
                     CV_HOUGH_GRADIENT,			// two-pass algorithm for detecting circles, this is the only choice available
                     2,							// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
                     matProcessed.rows / 4,		// min distance in pixels between the centers of the detected circles
                     100,						// high threshold of Canny edge detector (called by cvHoughCircles)
                     50,						// low threshold of Canny edge detector (set at 1/2 previous value)
                     10,						// min circle radius (any circles with smaller radius will not be returned)
                     400);						// max circle radius (any circles with larger radius will not be returned)

    for(itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++) {

        float x = (float)(*itrCircles)[0];
        float y = (float)(*itrCircles)[1];
        float r = (float)(*itrCircles)[2];
        ui->txtXYRadius->appendPlainText(QString("Ball position x =")+QString::number(x).rightJustified(4,' ')+
                                         QString(", y =")+QString::number(y).rightJustified(4,' ')+
                                         QString(", radius =")+QString::number(r,'f',3).rightJustified(7,' ')
                                         );

                                            // draw small green circle at center of detected object
        cv::circle(matOriginal,													// draw on original image
                   cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),		// center point of circle
                   3,															// radius of circle in pixels
                   cv::Scalar(0,255,0),											// draw pure green (remember, its BGR, not RGB)
                   CV_FILLED);													// thickness, fill in the circle

                                            // draw red circle around the detected object
        cv::circle(matOriginal,													// draw on original image
                   cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),		// center point of circle
                   (int)(*itrCircles)[2],										// radius of circle in pixels
                   cv::Scalar(0,0,255),											// draw pure red (remember, its BGR, not RGB)
                   3);															// thickness of circle in pixels
    }	// end for

    cv:cvtColor(matOriginal,matOriginal, CV_BGR2RGB);

    QImage qimgOriginal((uchar*)matOriginal.data,matOriginal.cols,matOriginal.rows,matOriginal.step,QImage::Format_RGB888);
    QImage qimgProcessed((uchar*)matProcessed.data,matProcessed.cols,matProcessed.rows,matProcessed.step,QImage::Format_Indexed8);

    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
    ui->lblProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));

}

