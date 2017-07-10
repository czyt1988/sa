#-------------------------------------------------
#
# Project created by czy 2016-11-04
# sa chart api lib
#
#-------------------------------------------------
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = signACommonUI
TEMPLATE = lib
CONFIG += dll

DESTDIR = ../bin/libs

CONFIG += c++11
QWT_CONFIG  += QwtDll
#czy
include($$PWD/../czy/czy.pri)
#propertybrowser
include($$PWD/../3rdParty/qtpropertybrowser/src/qtpropertybrowser.pri)#propertybrowser support
#sa api support
include($$PWD/../signALib/signALib.pri)
#QWT
include($$PWD/../3rdParty/qwt/qwt_set.pri)
#color picker support
include($$PWD/../3rdParty/qtcolorpicker/qtcolorpicker.pri)
#sa chart support
include($$PWD/../signAChart/signAChart.pri)

DEFINES += SA_COMMON_UI_MAKE #make dll


OTHER_FILES += readme.md

HEADERS += \
    SACommonUIGlobal.h \
    QtPropertyIdStorage.h \
    SAPropertySetDialog.h \
    SAUIReflection.h \
    SAFigureWindow.h \
    SAAbstractFigure.h \
    SAValueSelectDialog.h \
    SAUIInterface.h \
    SAUIHelper.h \
    SAMdiSubWindow.h \
    SAFigureContainer.h \
    QwtPlotPropertySetDialog.h \
    SAFiugreSetWidget.h \
    SAFigureCanvasSetWidget.h \
    SAPropertySetWidget.h

SOURCES += \
    SAPropertySetDialog.cpp \
    SAUIReflection.cpp \
    SAFigureWindow.cpp \
    SAAbstractFigure.cpp \
    SAValueSelectDialog.cpp \
    SAUIInterface.cpp \
    SAUIHelper.cpp \
    SAMdiSubWindow.cpp \
    SAFigureContainer.cpp \
    QwtPlotPropertySetDialog.cpp \
    SAFiugreSetWidget.cpp \
    SAFigureCanvasSetWidget.cpp \
    SAPropertySetWidget.cpp

FORMS += \
    SAValueSelectDialog.ui

#Chart2D
include($$PWD/Chart2D/Chart2D.pri)
#sa property item
include($$PWD/SAPropertyItem/SAPropertyItem.pri)
