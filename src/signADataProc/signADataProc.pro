#-------------------------------------------------
#
# Project created by QtCreator 2017-06-16 13:23:43
#
#-------------------------------------------------
message("")
message("--------------SA---------------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])

QT += core gui
QT += network
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


OTHER_FILES += readme.md

DESTDIR = ../bin


TARGET = signADataProc
TEMPLATE = app
CONFIG += c++11
INCLUDEPATH += $$PWD


SOURCES += main.cpp\
    SADataProcClient.cpp

HEADERS += \
    SADataProcClient.h
    


#sa api support
#{
#sa api support
include($$PWD/../signALib/signALib.pri)
#}


#the czy extern support
#{
include($$PWD/../czy/czy.pri)#the czy extern support
#}


