#-------------------------------------------------
#
# Project created by QtCreator 2011-10-24T13:56:22
#
#-------------------------------------------------

QT += \
    core \
    gui \
    xml \
    script

TARGET = SeraphScan
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scancontroller.cpp \
    mastercontrolunit.cpp \
    scandata.cpp


HEADERS += \
    mainwindow.h \
    scancontroller.h \
    mastercontrolunit.h \
    scandata.h


FORMS += \
    mainwindow.ui


INCLUDEPATH += FabConLib
include(FabConLib/FabConLib/include.pro)


#RESOURCES += fabprint.qrc
OTHER_FILES += SeraphScanner.config
#fabstudio.rc
#RC_FILE = fabstudio.rc

# OTHER_FILES should be referenced in include.pro.

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

