#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T15:17:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SAChartLibTest
TEMPLATE = app


SOURCES += main.cpp\
        Widget.cpp

HEADERS  += Widget.h

FORMS    += Widget.ui

include($$PWD/../../signAChart/signAChart.pri)
