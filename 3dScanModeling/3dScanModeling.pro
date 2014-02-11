#-------------------------------------------------
#
# Project created by QtCreator 2013-11-28T13:25:27
#
#-------------------------------------------------
QT += \
    core \
    gui \
    xml\
    script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3dScanModeling
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    glscanobject.cpp \
    vtkwidget.cpp \
	xygrid.cpp

HEADERS  += mainwindow.h \
    glscanobject.h \
    vtkwidget.h \
	xygrid.h

FORMS    += mainwindow.ui
include(common.pro)
include(shared.pro)
INCLUDEPATH += shared/math\
               shared/ \
               shared/utils



unix{
    #Eigen
    INCLUDEPATH += /usr/include/eigen3

    # OpenCV 2.4.5
    OpenCv = /home/theshadowx/software/OpenCV/OpenCv-2.4.5
    INCLUDEPATH += $$OpenCv/include/
    LIBS += -L$$OpenCv/lib \
            $$OpenCv/lib/libopencv_core.so.2.4 \
            $$OpenCv/lib/libopencv_imgproc.so.2.4 \
            $$OpenCv/lib/libopencv_highgui.so.2.4

    # PCL 1.7
    INCLUDEPATH += /usr/include/pcl-1.7/
    LIBS += -lpcl_registration \
            -lpcl_sample_consensus \
            -lpcl_features \
            -lpcl_filters \
            -lpcl_surface \
            -lpcl_segmentation \
            -lpcl_search \
            -lpcl_kdtree \
            -lpcl_octree \
            -lflann_cpp \
            -lpcl_common \
            -lpcl_io \
            -lpcl_visualization

    # VTK 5.8
    INCLUDEPATH += /usr/include/vtk-5.8
    LIBS += -L/usr/lib/ \
            -lvtkCommon  \
            -lvtksys \
            -lQVTK \
            -lvtkQtChart \
            -lvtkViews \
            -lvtkWidgets \
            -lvtkInfovis \
            -lvtkRendering \
            -lvtkGraphics  \
            -lvtkImaging  \
            -lvtkIO  \
            -lvtkFiltering \
            -lvtkDICOMParser \
            -lvtkalglib  \
            -lvtkverdict  \
            -lvtkmetaio  \
            -lvtkexoIIc  \
            -lvtkftgl  \
            -lvtkHybrid \
            -lboost_system

    #Build configuration
    ParentDirectory = ./build

    RCC_DIR = "$$ParentDirectory/RCCFiles"
    UI_DIR = "$$ParentDirectory/UICFiles"
    MOC_DIR = "$$ParentDirectory/MOCFiles"
    OBJECTS_DIR = "$$ParentDirectory/ObjFiles"


    CONFIG(debug, debug|release) {
        DESTDIR = "$$ParentDirectory/debug"
    }
    CONFIG(release, debug|release) {
        DESTDIR = "$$ParentDirectory/release"
    }
}

win32{
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

}
