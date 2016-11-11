#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T14:06:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = trackercv
TEMPLATE = app

INCLUDEPATH += C:\Dev\opencv_msvc\install\include
LIBS += -LC:\Dev\opencv_msvc\lib\Release -lopencv_core310 -lopencv_imgcodecs310 -lopencv_videoio310 -lopencv_video310 -lopencv_imgproc310 -lopencv_highgui310
LIBS += -LC:/Dev/eyetribe/lib -lGazeApiLib

INCLUDEPATH += C:/Dev/boost_1_62_0
LIBS += -LC:/Dev/boost_1_62_0/lib64-msvc-14.0 -llibboost_system-vc140-mt-1_62
LIBS += -LC:/Dev/boost_1_62_0/lib64-msvc-14.0 -llibboost_thread-vc140-mt-1_62
LIBS += -LC:/Dev/boost_1_62_0/lib64-msvc-14.0 -llibboost_chrono-vc140-mt-1_62

SOURCES += main.cpp \
    mainwindow.cpp \
    TrackingDataReceiver.cpp

HEADERS += \
    mainwindow.h \
    TrackingDataReceiver.h \
    eyetribe/gazeapi_observable.hpp \
    eyetribe/gazeapi_parser.hpp \
    eyetribe/gazeapi_socket.hpp \
    eyetribe/gazeapi.hpp \
    eyetribe/gazeapi_interfaces.hpp \
    eyetribe/gazeapi_types.hpp \
    ExtendedVideoWidget.h
