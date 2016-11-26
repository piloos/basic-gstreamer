#-------------------------------------------------
#
# Project created by QtCreator 2016-11-26T16:32:18
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = basic_pipeline
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include 
LIBS += -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0

TEMPLATE = app


SOURCES += main.cpp \
    pipeline.cpp

HEADERS += \
    pipeline.h
