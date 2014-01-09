#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



signals:
    void start();
    void stop();

public slots:
    void appendText(QString txt);
    void setScanState(bool started);
    void setImage(QPixmap q);
    void scanCompleted();


private slots:
    void on_startStopButton_clicked();


private:
    Ui::MainWindow *ui;
    bool started_;
};

#endif // MAINWINDOW_H
