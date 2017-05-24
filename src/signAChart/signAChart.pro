#-------------------------------------------------
#
# Project created by czy 2016-11-04
# sa chart api lib
#
#-------------------------------------------------
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = signAChart
TEMPLATE = lib
CONFIG += dll

DESTDIR = ../bin/libs

CONFIG += c++11



include($$PWD/../3rdParty/qwt/qwt_set.pri)
include($$PWD/../3rdParty/qtpropertybrowser/src/qtpropertybrowser.pri)#propertybrowser support

DEFINES += SA_CHART_MAKE #定义此变量后将会构建DEFINES += USE_QWT#定义绘图引擎使用qwt
#DEFINES += USE_QT_CHART#定义绘图引擎使用qt chart

HEADERS += \
    SAChartGlobals.h \
    SASeries.h \
    QwtPlotItemDataModel.h \
    QwtPlotItemTreeModel.h \
    QwtPlotPropertySetDialog.h \
    SAChart2D.h \
    SAChart3D.h \
    SAMarkSymbol.h \
    SAMultWidget.h \
    SAPlotMarker.h \
    SAXYDataTracker.h \
    SAYDataTracker.h \
    QtPropertyIDStorage.h \
    SAChart.h

SOURCES += \
    QwtPlotItemDataModel.cpp \
    QwtPlotItemTreeModel.cpp \
    QwtPlotPropertySetDialog.cpp \
    SAChart2D.cpp \
    SAChart3D.cpp \
    SAMarkSymbol.cpp \
    SAMultWidget.cpp \
    SAPlotMarker.cpp \
    SAXYDataTracker.cpp \
    SAYDataTracker.cpp \
    SAChart.cpp

OTHER_FILES += readme.md
