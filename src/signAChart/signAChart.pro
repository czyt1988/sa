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

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/libs
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/libs
}

CONFIG += c++11



include($$PWD/../3rdParty/qwt/qwt_set.pri)


DEFINES += SA_CHART_MAKE #定义此变量后将会构建DEFINES += USE_QWT#定义绘图引擎使用qwt
#DEFINES += USE_QT_CHART#定义绘图引擎使用qt chart

HEADERS += \
    SAChartGlobals.h \
    SASeries.h \
    QwtPlotItemDataModel.h \
    QwtPlotItemTreeModel.h \
    SAChart3D.h \
    SAMarkSymbol.h \
    SAPlotMarker.h \
    SAXYDataTracker.h \
    SAYDataTracker.h \
    SAChart.h \
    SA2DGraph.h \
    SAPlotZoomer.h \
    SASelectRegionShapeItem.h \
    SAAbstractPlotEditor.h \
    SAAbstractRegionSelectEditor.h \
    SARectRegionSelectEditor.h \
    SAEllipseRegionSelectEditor.h \
    SAPolygonRegionSelectEditor.h \
    SACrossTracker.h \
    SAQwtSerialize.h

SOURCES += \
    QwtPlotItemDataModel.cpp \
    QwtPlotItemTreeModel.cpp \
    SAChart3D.cpp \
    SAMarkSymbol.cpp \
    SAPlotMarker.cpp \
    SAXYDataTracker.cpp \
    SAYDataTracker.cpp \
    SAChart.cpp \
    SA2DGraph.cpp \
    SAPlotZoomer.cpp \
    SASelectRegionShapeItem.cpp \
    SAAbstractPlotEditor.cpp \
    SAAbstractRegionSelectEditor.cpp \
    SARectRegionSelectEditor.cpp \
    SAEllipseRegionSelectEditor.cpp \
    SAPolygonRegionSelectEditor.cpp \
    SACrossTracker.cpp \
    SAQwtSerialize.cpp

OTHER_FILES += readme.md
