#ifndef VTKWIDGET_H
#define VTKWIDGET_H

#include "glscanobject.h"

class VTKWidget : public QVTKWidget
{
    Q_OBJECT
public:
    explicit VTKWidget(QWidget *parent = 0);

    boost::shared_ptr<pcl::visualization::PCLVisualizer> m_pv;
    GlScanObject *glScanObject;

private:

signals:

public slots:

};

#endif // VTKWIDGET_H
