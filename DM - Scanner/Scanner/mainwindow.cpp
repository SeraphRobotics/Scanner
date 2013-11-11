#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <grid.h>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Function called when the "Find Directory" button is clicked
//Opens a directory chooser and stores selected path
//User is expected to select directory with XML scan file and scan images
void MainWindow::on_findDirectoryButton_clicked()
{
    //Open GUI File Chooser
    QString directoryPath = QFileDialog::getExistingDirectory();

    //Set Main Window GUI to visually display chosen file path
    this->ui->directoryLabel->setText(directoryPath);
}

//Function called when the "Process Scan" button is clicked
//Call primary class with selected directory and initialize post-processing scan
void MainWindow::on_processScanButton_clicked()
{
    //Retrieve the selected scan directory
    QString directoryPath = this->ui->directoryLabel->text();

    //Process Scan Data
    Grid grid(directoryPath); //Pass scan directory into grid
    bool success = grid.processScan(); //Execute post processing

    //Display Success Message
    if(success == true)
        qDebug() << "Scan Completed...";

}
