#-------------------------------------------------
#
# Project created by QtCreator 2016-11-16T11:40:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DESTDIR = ../../bin/libs
TARGET = SALibTest
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

include($$PWD/../../signALib/signALib.pri)
include($$PWD/../../3rdParty/qwt/qwt_set.pri)
include($$PWD/../../czy/czy.pri)#the czy extern support
