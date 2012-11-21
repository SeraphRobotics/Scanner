#-------------------------------------------------
#
# Project created by QtCreator 2012-11-16T13:24:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = OpenCVTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += C:\\opencv-2.4\\mybuild\\install\\include

LIBS += -LC:\\opencv-2.4\\mybuild\\install\\lib \
    -lopencv_core243.dll \
    -lopencv_highgui243.dll \
    -lopencv_imgproc243.dll \
    -lopencv_features2d243.dll \
    -lopencv_calib3d243.dll
