//Import Local Classes
#include "grid.h"

//Import OpenCV Classes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//Import QT Classes
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

using namespace cv;

//Constructor - Takes in scan directory which contains scan file and images
//Store in global variables for later use
Grid::Grid(QString directoryPath)
{
    //Initialize grid data
    this->directoryPath = directoryPath;
    this->scanFileName = "scan.xml";
}


//Main function to complete 3d scan post processing
//Return boolean - scan was completed successfully or not
bool Grid::processScan() {
    //Locate scan file - Create QFile object
    QFile scanFile(directoryPath + "/" + scanFileName);

    //Open scan file if it exists, return false if it does not
    if(scanFile.exists() == false || scanFile.open(QFile::ReadOnly | QFile::Text) == false)
        return false;

    //Create and Open XML Reader Object
    QXmlStreamReader xml(&scanFile);

    //Parse XML File as long as there are no errors and not at the end of the file
    while(xml.atEnd() == false && xml.hasError() == false) {
        //Read next element
        xml.readNext();

        //Parse XML tags
        if(xml.isStartElement() == true) {
            if(xml.name() == "line") {
                //Declare temp storage variables
                QString imgFileName = "";
                int xCoordinate = -1; //Physical x coordinate in 3d space

                //Retrieve & Store data from XML file
                xml.readNext();
                if(xml.name() == "x") {
                    xCoordinate = (int)xml.readElementText().toFloat(); //Store x value from XML file

                    xml.readNext(); //Continue reading xml file
                    if(xml.name() == "image") {
                        imgFileName = xml.readElementText(); // Store image name from XML file

                        //Pass retrieved data to image processing function
                        if(processImage(imgFileName, xCoordinate) == true)
                            qDebug() << imgFileName << " Processed Successfully";
                    }
                }
                else
                    qDebug() << "ERROR - Unable to process: " << imgFileName;
            }
        }
        else if(xml.hasError() == true)
            qDebug() << "XML Error: " << xml.errorString();
        else if(xml.atEnd() == true)
            qDebug() << "End of XML File";
    }

    //Close Scan File
    scanFile.close();

    //Return true, function completed successfully
    return true;
}

//Load and process Image & Data
bool Grid::processImage(QString imageFileName, int xCoordinate) {
    //Load Image
    QString imgPath = directoryPath + "/" + imageFileName + ".jpg";
    Mat img = imread(imgPath.toStdString() , CV_LOAD_IMAGE_UNCHANGED);

    //Return false if image is not able to load properly
    if(img.data == NULL)
        return false;

    //Convert image to grayscale - Simpler to process
    cvtColor(img, img, CV_BGR2GRAY);

    //Use binary thresholding - Convert relevant values to full white (255)
    int thresholdValue = 120;
    threshold(img, img, thresholdValue, 255, 0);

    //Parse image for white pixels (x, y) and add to global vector<Point> array
    for (int y = 0; y < img.rows; y++ ) {
        for (int x = 0; x < img.cols; x++) {
            //Check if curren't pixel is white
            if (img.at<uchar>(y, x) == 255) {
                //Store all white pixels (x', y') from the processed image to global list
                xyCoordinates.push_back(Point(x, y));

                //Calculate and Store 3D Coordinates (x, y, z) using known data to global list
                //x - Given by motor encoder
                //y - unknown
                //z - unknown
                coordinates.push_back(Point3i(xCoordinate, -1, -1));
            }
        }
    }

    //Display Processed image - TESTING
    //imshow(imageFileName.toStdString(), img);

    //Create .asc file - TESTING IN MESHLAB
    createASCFile();

    //Return true - Successfully processed image
    return true;
}

//Create a 3d Model ".asc" file - contains only coordinates (not meshed)
//Open file in Meshlab for testing
bool Grid::createASCFile() {
    //Create QFile for output .asc file in scan directory
    QFile outFile(directoryPath + "/out.asc");

    //Delete file if it already exists
    outFile.remove();

    //Recreate file and open output stream -> write to file
    outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&outFile);

    //Print (x, y, z) coordinates to output file
    for(int i = 0; i < (int)coordinates.size(); i++)
      stream << coordinates[i].x << ", " << coordinates[i].y << ", " << coordinates[i].z << endl; //FORMAT REQUIRED FOR .ASC FILE

    //Close Output File
    outFile.close();

    //Return successful
    return true;
}
