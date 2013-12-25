#include <QFileDialog>
#include <QDebug>
#include <QGraphicsScene>
#include <qmath.h>
#include <QFile>
#include <QDir>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initilize VTK widget
    vtkWidget = ui->vtkWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pBLoad_clicked()
{
    // Get the files
    QFileDialog dialog(this);
    QStringList fileNames;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
    }else{
        return;
    }
    // extract the point cloud of the laser from the images
    vtkWidget->m_pv->removeAllPointClouds();
    vtkWidget->glScanObject->extractPointCloud(fileNames);
}

void MainWindow::on_pBSave_clicked()
{
    // Getting the point cloud vector
    pcl::PointCloud<pcl::PointXYZ>::Ptr pPointCloud = vtkWidget->glScanObject->getPointCloud();
    // Checking if the vector is empty
    if(pPointCloud->empty()){
        QMessageBox::warning(this, tr("Warning"),
                                        tr("No cloud points to be saved"),
                                        QMessageBox::Ok);
        return;
    }
    // Open a dialog to specify where the point cloud should be saved
    QFileDialog dialog;
    QString savefilePath = dialog.getSaveFileName(this,"Save...",QDir::homePath(),"Text files (*.txt)");
    if(savefilePath.isEmpty())
        return;

    QFile saveFile(savefilePath);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // Write the coordinate of each point in the specified file
    QTextStream textStream(&saveFile);

    for(int i=0; i<pPointCloud->size(); i++)
        textStream << pPointCloud->points[i].x << ", " << pPointCloud->points[i].y << ", " << pPointCloud->points[i].z << endl;

    saveFile.close();
}
