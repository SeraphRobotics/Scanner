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


INCLUDEPATH += C:\\OpenCV\\build_with_Qt\\release\\include

LIBS += -LC:\\OpenCV\\build_with_Qt\\release\\lib \
    -lopencv_calib3d245.dll\
    -lopencv_contrib245.dll\
    -lopencv_core245.dll \
    -lopencv_features2d245.dll \
    -lopencv_flann245.dll \
    -lopencv_gpu245.dll \
    -lopencv_highgui245.dll \
    -lopencv_imgproc245.dll \
    -lopencv_legacy245.dll \
    -lopencv_ml245.dll\
    -lopencv_objdetect245.dll \
    -lopencv_ts245 \
    -lopencv_video245.dll
