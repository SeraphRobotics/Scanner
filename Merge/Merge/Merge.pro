#-------------------------------------------------
#
# Project created by QtCreator 2014-10-05T20:10:43
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Merge
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    scanmerger.cpp


INCLUDEPATH += C:\\opencv\\build_with_Qt\\release\\include

#LIBS += -LC:\\opencv\\build_with_Qt\\release\\lib \
#    -lopencv_calib3d245.dll\
#    -lopencv_contrib245.dll\
#    -lopencv_core245.dll \
#    -lopencv_features2d245.dll \
#    -lopencv_flann245.dll \
#    -lopencv_gpu245.dll \
#    -lopencv_highgui245.dll \
#    -lopencv_imgproc245.dll \
#    -lopencv_legacy245.dll \
#    -lopencv_ml245.dll\
#    -lopencv_objdetect245.dll \
#    -lopencv_ts245 \
#    -lopencv_video245.dll

LIBS += -LC:\\opencv\\build_with_Qt\\release\\lib \
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

HEADERS += \
    scanmerger.h

