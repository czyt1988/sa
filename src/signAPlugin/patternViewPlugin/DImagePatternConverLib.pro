#-------------------------------------------------
#
# 图片转花样的核心算法类封装
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DESTDIR = ../bin/
TARGET = DImagePatternConverLib
TEMPLATE = lib
CONFIG += staticlib
#DEFINES += D_IMAGE2PATTERN_USE_SHARE_LIB #使用动态库
DEFINES += D_IMAGE2PATTERN_MAKE #make lib
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD


SOURCES += \
    DImagePatternConver.cpp \
    DImageAlgorithm.cpp

HEADERS += \
    $$PWD/headers/DImagePatternConver.h \
    $$PWD/headers/DImagePatternConverGlobal.h \
    $$PWD/headers/DImageAlgorithm.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
