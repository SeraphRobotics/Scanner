#-------------------------------------------------
#
# Project created by QtCreator 2012-11-15T15:46:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CameraTest
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

INCLUDEPATH += C:\\OpenCV-2.4\\mybuild\\install\\include

LIBS += -LC:\\OpenCV-2.4\\mybuild\\install\\lib \
    -lopencv_calib3d243.dll\
    -lopencv_contrib243.dll\
    -lopencv_core243.dll \
    -lopencv_features2d243.dll \
    -lopencv_flann243.dll \
    -lopencv_gpu243.dll \
    -lopencv_highgui243.dll \
    -lopencv_imgproc243.dll \
    -lopencv_legacy243.dll \
    -lopencv_ml243.dll\
    -lopencv_objdetect243.dll \
    -lopencv_ts243 \
    -lopencv_video243.dll
