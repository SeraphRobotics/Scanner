#ifndef GRID_H
#define GRID_H

#include <QString>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

class Grid
{
public:
    //Constructor
    Grid(QString directoryPath); //Initialize scan directory and scanfile name

    //Primary Function
    bool processScan(); //Function called to initialize and complete post-processing scan
private:
    //Global Variables
    QString directoryPath; //Stores the working directory of the scan
    QString scanFileName; //Stores file name of XML scan file
    std::vector<cv::Point> xyCoordinates; //List of (x,y) points where the laser hits the image
    std::vector<cv::Point3i> coordinates; //list of final 3D coordinates

    //Function called to process image and store relevant data in global lists
    bool processImage(QString imageFileName, int xCoordinate);

    //Create a 3D Model File - Stores only points
    bool createASCFile();
};

#endif // GRID_H
