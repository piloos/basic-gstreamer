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

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0  #equivalent of pkg-config use (eg. pkg-config --cflags --libs)

TEMPLATE = app

SOURCES += main.cpp \
    pipeline.cpp

HEADERS += \
    pipeline.h
