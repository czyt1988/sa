INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
# user need add 
#include($$PWD/../3rdParty/qwt/qwt_set.pri)
# user need add 
#include($$PWD/../3rdParty/qtpropertybrowser/src/qtpropertybrowser.pri)#propertybrowser support
DEFINES -= SA_CHART_MAKE #定义此变量后将会构建库
DEFINES += USE_QWT
CONFIG += qwt
QWT_CONFIG  += QwtDll

#HEADERS += \
#    $$PWD/SAChartGlobals.h \
#    $$PWD/SASeries.h \
#    $$PWD/QwtPlotItemDataModel.h \
#    $$PWD/QwtPlotItemTreeModel.h \
#    $$PWD/QwtPlotPropertySetDialog.h \
#    $$PWD/SAChart2D.h \
#    $$PWD/SAChart3D.h \
#    $$PWD/SAMarkSymbol.h \
#    $$PWD/SAMultWidget.h \
#    $$PWD/SAPlotMarker.h \
#    $$PWD/SAXYDataTracker.h \
#    $$PWD/SAYDataTracker.h \
#    $$PWD/QtPropertyIDStorage.h \
#    $$PWD/SAAbstractFigure.h\
#    $$PWD/SAChart.h

    LIBS += -L$$PWD/../bin/libs/ -lsignAChart
