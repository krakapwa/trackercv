#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T14:06:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvVideoPlayer
TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += /usr/include/opencv

LIBS += -L/usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lopencv_video -lopencv_imgproc -lopencv_highgui
