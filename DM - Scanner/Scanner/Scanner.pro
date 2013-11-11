#-------------------------------------------------
#
# Project created by QtCreator 2013-11-10T14:08:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    grid.cpp

HEADERS  += mainwindow.h \
    grid.h

FORMS    += \
    mainwindow.ui

INCLUDEPATH +=  /usr/include/opencv \
                /usr/include/pcl-1.7 \
                /usr/include/eigen3 \
                /usr/include/boost \

LIBS += -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab \
        -lboost_system
