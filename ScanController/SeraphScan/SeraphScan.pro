#-------------------------------------------------
#
# Project created by QtCreator 2011-10-24T13:56:22
#
#-------------------------------------------------

QT += \
    core \
    gui \
    xml\
    script

CONFIG +=GUI

TARGET = SeraphScan
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scancontroller.cpp \
    mastercontrolunit.cpp \
    scandata.cpp\
	scanprocessing.cpp\
    xygrid.cpp


HEADERS += \
    mainwindow.h \
    scancontroller.h \
    mastercontrolunit.h \
    scandata.h\
	scanprocessing.h\
    xygrid.h


FORMS += \
    mainwindow.ui

	
INCLUDEPATH += ../

include(common.pro)
include(shared.pro)
INCLUDEPATH += ../shared/math\
               ../shared/ \
               ../shared/utils
	

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

