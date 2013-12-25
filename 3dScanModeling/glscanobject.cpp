#include "glscanobject.h"

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

    //set-up camera parameters
    //TODO - change these to the actual values
    double xFOV = 43.7;     // horizontal field of view
    double yFOV = 23.8;     // vertical field of view
    double f = 3.6;         // focal length
    double kv;              // ku, kv the inverses
    double ku;              // of the dimensions of a pixel
    double u0;              // (u0,v0) the coordinates
    double v0;              // of the principal point

    cv::Mat coefDist;       // Coef distortion matrix

    double h = 184.3;       // the distance the laser and the plate
    double b = 183;         // distance between camera and laser

    double xOff = 28;       //  Offset

    double z;               //  Z
    double y;               //  Y


    // Clear the Vector
    m_pointCloud3d->clear();

    // read the image
    for(int j=0; j<ImagesPath.count(); j++){
        img = cv::imread(ImagesPath[j].toStdString() , CV_LOAD_IMAGE_UNCHANGED);

        if(img.data == NULL){
            qDebug() << "ERROR : IMG";
            return;
        }

        u0 = img.cols/2;

        /// ////////////////////////////////////////////
        cv::blur( img, img, cv::Size( 3, 3), cv::Point(-1,-1));

        // Transform the color to GrayScale
        cvtColor(img, img, CV_BGR2GRAY);

        int thresholdValue = 90;
        cv::threshold(img, img, thresholdValue, 255, 0);
        //cv::blur( img, img, cv::Size(3,3) );


        /// ///////////////////////////////
        for (int v = 0; v < img.rows; v++ ){
            for (int u = 0; u < img.cols; u++){
                if (img.at<uchar>(v, u) > 0) {

                        z = (u0+xOff) - u;
                        y = v;

                     if(!metOne){
                        m_pointCloud3d->push_back(pcl::PointXYZ(j,y,z));
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
