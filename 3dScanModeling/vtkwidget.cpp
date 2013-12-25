#include "vtkwidget.h"

VTKWidget::VTKWidget(QWidget *parent) :
    QVTKWidget(parent)
{
    m_pv = boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer("3D Viewer",false) );
    m_pv->addCoordinateSystem(100);
    vtkSmartPointer<vtkRenderWindow> renderWindow = m_pv->getRenderWindow();
    this->SetRenderWindow( renderWindow );
    this->update();

    glScanObject = new GlScanObject(this,m_pv);
}
