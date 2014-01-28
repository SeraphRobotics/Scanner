#-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T12:05:12
#
#-------------------------------------------------
DEFINES += CONFIRM_ENABLED
QT       += core\
         script\
         xml


#QT       -= gui

TARGET = ScanProcessing
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    scanprocessing.cpp\
#    xygrid.cpp

HEADERS += \
    scanprocessing.h\
#    xygrid.h

INCLUDEPATH += ../

include(common.pro)
include(shared.pro)
INCLUDEPATH += ../shared/math\
               ../shared/ \
               ../shared/utils


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

    #Build configuration
    ParentDirectory = ./build

    RCC_DIR = "$$ParentDirectory\\RCCFiles"
    UI_DIR = "$$ParentDirectory\\UICFiles"
    MOC_DIR = "$$ParentDirectory\\MOCFiles"
    OBJECTS_DIR = "$$ParentDirectory\\ObjFiles"


    CONFIG(debug, debug|release) {
        DESTDIR = "$$ParentDirectory\\debug"
    }
    CONFIG(release, debug|release) {
        DESTDIR = "$$ParentDirectory\\release"
    }
