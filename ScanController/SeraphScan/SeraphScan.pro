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

