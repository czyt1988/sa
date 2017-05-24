#-------------------------------------------------
#
# Project created by QtCreator 2015-09-07T10:36:27
#
#-------------------------------------------------

QT       -= gui

TARGET = QTDM
TEMPLATE = lib

DEFINES += QTDM_LIBRARY

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD


SOURCES += $$PWD/QTDM.cpp\
    $$PWD/QTDMFile.cpp \
    $$PWD/QTDMGroup.cpp \
    $$PWD/QTDMChannel.cpp \
    $$PWD/QTDMChannelTableModel.cpp

HEADERS += \
    $$PWD/nilibddc.h\
    $$PWD/QTDM.h\
    $$PWD/qtdm_global.h\
    $$PWD/QTDMFile.h \
    $$PWD/QTDMGroup.h \
    $$PWD/QTDMChannel.h \
    $$PWD/QTDMChannelTableModel.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$PWD/ -lnilibddc



