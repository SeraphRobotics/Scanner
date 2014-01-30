#ifndef GLSCANOBJECT_H
#define GLSCANOBJECT_H

//Import Qt Headers
#include <QObject>
#include <QVector>
#include <QStringList>
#include <QColor>
#include <QPair>
#include <QFile>
#include <QDir>
#include <QDebug>

//Import OpenCV Headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//Import VTK Headers
#include <QVTKWidget.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

//Import PCL Headers
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/common/common_headers.h>
#include <pcl/features/integral_image_normal.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>

class GlScanObject:public QObject
{
public:
    // constructors
    GlScanObject(QObject *parent=0);
    GlScanObject(QVTKWidget *vtkWidget, boost::shared_ptr<pcl::visualization::PCLVisualizer> pv, QObject *parent=0);


    pcl::PointCloud<pcl::PointXYZ>::Ptr getPointCloud() const;      // get the point cloud

    void extractPointCloud(const QStringList &ImagesPath);          // extract point cloud from image
    void extractCorners();                                          // extract the corners of the object
    void extractNormals();                                          // estimate the normals
    void extractTrianglesMesh();                                    // generate the mesh

    QPair<double,double> getXMinMax() const;                        // get the min/max X coordinate of the point cloud
    QPair<double,double> getYMinMax() const;                        // get the min/max Y coordinate of the point cloud
    QPair<double,double> getZMinMax() const;                        // get the min/max Z coordiante of the point cloud

private:

    double m_Xmin;
    double m_Xmax;
    double m_Ymin;
    double m_Ymax;
    double m_Zmin;
    double m_Zmax;

    boost::shared_ptr<pcl::visualization::PCLVisualizer> m_pv;      // the visulizer
    pcl::PointCloud<pcl::PointXYZ>::Ptr m_pointCloud3d;             // the point cloud
    pcl::PointCloud<pcl::Normal>::Ptr m_normals;                    // the normals
    pcl::PolygonMesh m_triangles;                                   // mesh triangles

    QVTKWidget *m_vtkWidget;                                        // vtk widget

};

#endif // GLSCANOBJECT_H
