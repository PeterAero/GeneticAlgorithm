#-------------------------------------------------
#
# Project created by QtCreator 2017-12-09T13:31:19
#
#-------------------------------------------------

QT       += core

QT       -= gui

INCLUDEPATH += /usr/include/gdal
LIBS += -L/usr/lib -lgdal

TARGET = RW
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
