#include "scanprocessing.h"
#include <QFileInfo>
#include <QDebug>
#include <math.h>

ScanProcessing::ScanProcessing(QObject *parent) :
    QObject(parent), dir_(QDir::currentPath()),extension_("")
{



}

void ScanProcessing::setDir(QString dir){

    dir_=dir;

    extension_="JPEG";
    QStringList name_filters;
    name_filters<< "*"+extension_; //get from QSettings

    dir_.setFilter(QDir::Files|QDir::NoDotAndDotDot);
    dir_.setNameFilters(name_filters);

    filenames_ = dir_.entryList();
}

void ScanProcessing::processScan(){
    cv::Mat img;
    bool metOne = false;
    QFileInfo fileInfo;

    /// ////////////////////////////////////////////////////////////////
    //set-up camera parameters
    //TODO - change these to the actual values
    double fu = 1388;                               // (fu,fv) are the focal lengths
    double fv = 1388;                               // expressed in pixel-related units
    double u0 = 970;                                // (u0,v0) the coordinates
    double v0 = 553;                                // of the principal point [pxl]
    double k1 = 0.102;                              // (k1, k2, k3, k4, k5 ,k6)
    double k2 = -0.170;                             // radial distortion coefficients
    double k3 = 0;
    double k4 = 0;
    double k5 = 0;
    double k6 = 0;
    double p1 = 0;                                  //(p1,p2) tangential distortion coefficients
    double p2 = 0;                                  //
    double h = 203.14;                              // the distance the laser and the surface
    double b = 182.54;                              // distance between camera and laserplate

    double uOff = 28;                               //  horizontal Offset
    double vOff = 28;                               //  vertical Offset

    double alpha = 225*CV_PI/180.;
    double beta  = 0*CV_PI/180.;
    double gamma = 180*CV_PI/180.;

    double z;                                       //  Z
    double y;                                       //  Y
    double x;                                    //  X


    // Camera matrix
    cv::Mat K = (cv::Mat_<double>(4, 4) <<
                 -fu,     0,         0,      u0,
                 0,       fv,        0,      v0,
                 0,       0,         1,      0,
                 0,       0,         0,      1);

    // Distortion matrix
    cv::Mat distCoef = (cv::Mat_<double>(8, 1) <<
                        k1,
                        k2,
                        p1,
                        p2,
                        k3,
                        k4,
                        k5,
                        k6);

    // Rotation matrices around the X, Y and Z axis
    cv::Mat RX = (cv::Mat_<double>(4, 4) <<
                  1,          0,           0,   0,
                  0, cos(alpha), -sin(alpha),   0,
                  0, sin(alpha),  cos(alpha),   0,
                  0,          0,           0,   1);

    cv::Mat RY = (cv::Mat_<double>(4, 4) <<
                  cos(beta), 0, -sin(beta), 0,
                  0, 1,          0, 0,
                  sin(beta), 0,  cos(beta), 0,
                  0, 0,          0, 1);

    cv::Mat RZ = (cv::Mat_<double>(4, 4) <<
                  cos(gamma), -sin(gamma), 0, 0,
                  sin(gamma),  cos(gamma), 0, 0,
                  0,          0,           1, 0,
                  0,          0,           0, 1);

    // translation matrix
    cv::Mat T = (cv::Mat_<double>(4, 4) <<
                 1,    0,  0,   b,
                 0,    1,  0,   0,
                 0,    0,  1,   -h,
                 0,    0,  0,   1);

    /// /////////////////////////////////////////////////////////////

    // Clear the Vector
//    m_pointCloud3d->clear();

    QStringList ImagesPath = filenames_;
    // read the image
    for(int j=0; j<ImagesPath.count(); j++){

        QVector< FAHVector3 >* row = new QVector< FAHVector3 >();

        QString location = dir_.path()+QString("//")+ImagesPath[j];
        qDebug()<<location;
        img = cv::imread(location.toStdString() , CV_LOAD_IMAGE_UNCHANGED);
        fileInfo.setFile(ImagesPath[j]);
        if(img.data == NULL){
            qDebug() << "ERROR : IMG";
            return;
        }

        //        u0 = img.cols/2;
        //        v0 = img.rows/2;

        /// ////////////////////////////////////////////
        cv::medianBlur ( img, img, 7 );

        // Transform the color to GrayScale
        cv::cvtColor(img, img, CV_BGR2GRAY);

        // Binary transform
        int thresholdValue = 45;
        cv::threshold(img, img, thresholdValue, 255, 0);

        /// ///////////////////////////////
        for (int u = 0; u < img.cols; u++){
            for (int v = 0; v < img.rows; v++ ){
                if (img.at<uchar>(v, u) > 0) {
                    if(!metOne){
                        // Undistort
                        double x_dist = (u - u0) / fu;
                        double y_dist = (v - v0) / fv;

                        double r = sqrt(pow(x_dist,2) + pow(y_dist,2));

                        double x_undist = x_dist*(1 + k1*pow(r,2) + k2*pow(r,4)+ k3*pow(r,6))/(1 + k4*pow(r,2) + k5*pow(r,4) + + k6*pow(r,6))
                                + 2*p1*x_dist*y_dist + p2*(pow(r,2) + 2*pow(x_dist,2));
                        double y_undist = y_dist*(1 + k1*pow(r,2) + k2*pow(r,4)+ k3*pow(r,6))/(1 + k4*pow(r,2) + k5*pow(r,4) + + k6*pow(r,6))
                                + p1*(pow(r,2)+2*pow(y_dist,2)) + 2*p2*x_dist*y_dist;

                        double u_undist = fu*x_undist + u0;
                        double v_undist = fv*y_undist + v0;

                        // Calculate the world coordinates
                        z = h-((fv*h-(v0-v_undist)*b)/(fv*b+h*(v0-v_undist)))*b ;
                        y = (u_undist-u0)*sqrt(h*h+b*b)/fu;

                        QString first = ImagesPath[j].replace("."+extension_,"");
//                        qDebug()<<first;
                        x = first.toFloat();//fileInfo.baseName().toDouble();

                        FAHVector3 v;
                        v.x=x;
                        v.y=y;
                        v.z=z;
                        row->push_back(v);
//                        m_pointCloud3d->push_back(pcl::PointXYZ(x,y,z));
                        metOne = true;
                    }
                }
            }
            metOne = false;
        }
        pointCloud[row->at(0).x]=row;
    }

    // visualize the point cloud
//    m_pv->addPointCloud(m_pointCloud3d);
}


//QDebug operator<<(QDebug dbg,FAHVector3 v ){
//    dbg.nospace()<<"(" <<v.x<<","<<v.y <<","<<v.z <<")";
//    return dbg.space();
//}

XYGrid<float> *ScanProcessing::makeGrid(){
    float GRID_SIZE = 2; /// need to save from elsewhere
    float Tolerance = 0.3;
    float max_x=0;
    float min_x=0;
    float max_y=0;
    float min_y=0;
    float min_z=0;

    QList<float> xs= pointCloud.keys();
    for (int i=0;i<xs.size();i++){
        if (xs[i]<min_x){min_x = xs[i];}
        if (xs[i]>max_x){max_x = xs[i];}

        QVector< FAHVector3 >* row = pointCloud.value(xs[i]);
        for(int j=0;j<row->size();j++){
            if(max_y<row->at(j).y){max_y=row->at(j).y;}
            if(min_y>row->at(j).y){min_y=row->at(j).y;}
            if(min_z>row->at(j).z){min_z=row->at(j).z;}
        }
    }

    int nx = ceil((max_x-min_x)/GRID_SIZE);
    int ny = ceil((max_y-min_y)/GRID_SIZE);

    qDebug()<<"NX: "<<nx<<" NY: "<<ny;
    qDebug()<<"minX: "<<min_x<<" minY: "<<min_y;
    qDebug()<<"maxX: "<<max_x<<" maxY: "<<max_y;
    qDebug()<<"minZ: "<<min_z;

    XYGrid<float>* grid = new XYGrid<float>(nx,ny,GRID_SIZE);

    for(int i=0; i<nx;i++){

        // Sort through each row and find the closest row to this grid.
//        qDebug()<<"i:"<<i;
        float x=-1;
        for(int j=0;j<xs.size();j++){
            if(   (xs.at(j)<(i*GRID_SIZE+Tolerance)) && (xs.at(j)>(i*GRID_SIZE-Tolerance) )){
                x=xs.at(j);
//                qDebug()<<"x:"<<i*GRID_SIZE <<"\tx'"<<xs.at(j);
            }
        }

        if(x<0){continue;} /// If no row continue to next row
        QVector< FAHVector3 >* row = pointCloud.value(x);

        // For that row find the point closest to the Y point
        for(int j=0; j<ny; j++){
//            float y = j*GRID_SIZE;// target point

            grid->operator ()(i,j)=0;

            for(int k=0;k<row->size();k++){

                float y_p = row->at(k).y-min_y; //compensate for negative min_y

                if(   (y_p<(j*GRID_SIZE+Tolerance)) && (y_p>(j*GRID_SIZE-Tolerance) )){
                    grid->operator ()(i,j)=row->at(k).z-min_z;
//                    qDebug()<<i<<","<<j<<":"<<row->at(k).z-min_z;
                    k=row->size();
                }

            }
        }


    }

    return grid;
}
