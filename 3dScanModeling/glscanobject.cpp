#include "glscanobject.h"
#include <QFileInfo>

///////////// Global functions /////////////////////

bool sortingXI(pcl::PointXYZ pointA,pcl::PointXYZ pointB)
{
    return pointA.x <pointB.x;
}

bool sortingYI(pcl::PointXYZ pointA,pcl::PointXYZ pointB)
{
    return pointA.y <pointB.y;
}

bool sortingZI(pcl::PointXYZ pointA,pcl::PointXYZ pointB)
{
    return pointA.z <pointB.z;
}


////////////////////////////////////////////
GlScanObject::GlScanObject(QObject *parent)
    : QObject(parent)
{
    m_Xmin = 0;
    m_Xmax = 0;
    m_Ymin = 0;
    m_Ymax = 0;
    m_Zmin = 0;
    m_Zmax = 0;

    m_pointCloud3d =  pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>());
    m_normals = pcl::PointCloud<pcl::Normal>::Ptr(new pcl::PointCloud<pcl::Normal>);

}

GlScanObject::GlScanObject(QVTKWidget *vtkWidget, boost::shared_ptr<pcl::visualization::PCLVisualizer> pv,QObject *parent)
    : QObject(parent)
{
    m_Xmin = 0;
    m_Xmax = 0;
    m_Ymin = 0;
    m_Ymax = 0;
    m_Zmin = 0;
    m_Zmax = 0;

    m_vtkWidget = vtkWidget;
    m_pv = pv;

    m_pointCloud3d =  pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>());
    m_normals = pcl::PointCloud<pcl::Normal>::Ptr(new pcl::PointCloud<pcl::Normal>);
}

pcl::PointCloud<pcl::PointXYZ>::Ptr GlScanObject::getPointCloud() const
{
    return m_pointCloud3d;
}

void GlScanObject::extractPointCloud(const QStringList &ImagesPath)
{
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
    double x;                                       //  X


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
    m_pointCloud3d->clear();

    // read the image
    for(int j=0; j<ImagesPath.count(); j++){
        img = cv::imread(ImagesPath[j].toStdString() , CV_LOAD_IMAGE_UNCHANGED);
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
                        x = fileInfo.baseName().toDouble();

                        m_pointCloud3d->push_back(pcl::PointXYZ(x,y,z));
                        metOne = true;
                    }
                }
            }
            metOne = false;
        }
    }
    // visualize the point cloud
    m_pv->addPointCloud(m_pointCloud3d);
}

void GlScanObject::extractCorners()
{
    // minX / maxX
    qSort(m_pointCloud3d->begin(),m_pointCloud3d->end(),sortingXI);
    m_Xmin = m_pointCloud3d->at(0).x;
    m_Xmax = m_pointCloud3d->at(m_pointCloud3d->size()-1).x;
    // minY / maxY
    qSort(m_pointCloud3d->begin(),m_pointCloud3d->end(),sortingYI);
    m_Ymin = m_pointCloud3d->at(0).y;
    m_Ymax = m_pointCloud3d->at(m_pointCloud3d->size()-1).y;
    // minZ / maxZ
    qSort(m_pointCloud3d->begin(),m_pointCloud3d->end(),sortingZI);
    m_Zmin = m_pointCloud3d->at(0).z;
    m_Zmax = m_pointCloud3d->at(m_pointCloud3d->size()-1).z;
}

void GlScanObject::extractNormals()
{
    // Normal estimation*
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud (m_pointCloud3d);
    ne.setInputCloud (m_pointCloud3d);
    ne.setSearchSurface(m_pointCloud3d);
    ne.setSearchMethod(tree);
    ne.setKSearch(4);
    ne.setViewPoint(0,0,1);
    //    ne.setKSearch (20); //20
    ne.compute (*m_normals);

}

void GlScanObject::extractTrianglesMesh()
{
    if(m_normals->size() == 0){
        qDebug()<< "Empty normals";
        return;
    }

    // Concatenate the XYZ and normal fields*
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
    pcl::concatenateFields (*m_pointCloud3d, *m_normals, *cloud_with_normals);
    //* cloud_with_normals = cloud + normals

    // Create search tree*
    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud (cloud_with_normals);

    // Initialize objects
    pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;

    // Set the maximum distance between connected points (maximum edge length)
    gp3.setSearchRadius (100);

    // Set typical values for the parameters
    gp3.setMu (2.5);
    gp3.setMaximumNearestNeighbors (10000);
    gp3.setMaximumSurfaceAngle(M_PI/16);
    gp3.setMinimumAngle(M_PI/18);
    gp3.setMaximumAngle(2*M_PI/3);
    gp3.setNormalConsistency(false);

    // Get result
    gp3.setInputCloud (cloud_with_normals);
    gp3.setSearchMethod (tree2);
    gp3.reconstruct (m_triangles);
}

QPair<double, double> GlScanObject::getXMinMax() const
{
    return QPair<double,double>(m_Xmin,m_Xmax);
}

QPair<double, double> GlScanObject::getYMinMax() const
{
    return QPair<double,double>(m_Ymin,m_Ymax);
}

QPair<double, double> GlScanObject::getZMinMax() const
{
    return QPair<double,double>(m_Zmin,m_Zmax);
}
