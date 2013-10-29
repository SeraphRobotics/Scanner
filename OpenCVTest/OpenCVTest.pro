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

INCLUDEPATH += C:\\OpenCV\\build_with_Qt\\release\\include

LIBS += -LC:\\OpenCV\\build_with_Qt\\release\\lib \
    -lopencv_core245.dll \
    -lopencv_highgui245.dll \
    -lopencv_imgproc245.dll \
    -lopencv_features2d245.dll \
    -lopencv_calib3d245.dll
