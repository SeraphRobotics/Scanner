#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),started_(0)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::appendText(QString txt)
{
    ui->statusTextEdit->appendPlainText(QString("\n")+txt);
}

void MainWindow::setScanState(bool started){
    started_ = started;
    if ((started_)){
        ui->startStopButton->setText("Stop");
        ui->statusTextEdit->clear();
    }else{
        ui->startStopButton->setText("Start");
    }
}

void MainWindow::on_startStopButton_clicked()
{
    if (!(started_)){
        emit start();
    }else{
        emit stop();
    }
}
