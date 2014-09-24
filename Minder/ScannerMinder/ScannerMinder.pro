#-------------------------------------------------
#
# Project created by QtCreator 2014-05-13T19:47:34
#
#-------------------------------------------------

QT       += core\
            xml

QT       -= gui

TARGET = ScannerMinder
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../qdevicewatcher-master/src/libQDeviceWatcher.pri)

win32:!wince*:LIBS += -lUser32

SOURCES += main.cpp \
    recordkeeper.cpp \
    usbminder.cpp \
    scannerarduinointerface.cpp \
    scannerwatcher.cpp \
    scannercontroller.cpp

HEADERS += \
    recordkeeper.h \
    usbminder.h \
    scannerarduinointerface.h \
    scannerwatcher.h \
    scannercontroller.h

include("../qextserialport-1.2rc/src/qextserialport.pri")
INCLUDEPATH += ../qextserialport/



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
