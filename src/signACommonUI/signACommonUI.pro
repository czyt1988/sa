#-------------------------------------------------
#
# Project created by czy 2016-11-04
# sa chart api lib
#
#-------------------------------------------------
QT += core gui xml
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
#CTK
include($$PWD/../3rdParty/ctk/ctk.pri)
#QTI
include($$PWD/../3rdParty/qti/qti.pri)
DEFINES += SA_COMMON_UI_MAKE #make dll
DEFINES += SA_COMMON_UI_USE_DYNAMIC #定义使用dynamic_cast 而不是rtti

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
    SAFiugreSetWidget.h \
    SAPropertySetWidget.h \
    TestWidget.h \
    SAPenSetWidget.h \
    SAColorPickerButton.h \
    SAPenStyleComboBox.h \
    SAFontSetWidget.h \
    SAAligmentSetWidget.h \
    SAValueManagerTreeView.h \
    SATendencyChartDataSelectDialog.h \
    SAMdiSubWindowSerializeHead.h \
    SAProjectManager.h

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
    SAFiugreSetWidget.cpp \
    SAPropertySetWidget.cpp \
    TestWidget.cpp \
    SAPenSetWidget.cpp \
    SAColorPickerButton.cpp \
    SAPenStyleComboBox.cpp \
    SAFontSetWidget.cpp \
    SAAligmentSetWidget.cpp \
    SAValueManagerTreeView.cpp \
    SATendencyChartDataSelectDialog.cpp \
    SAMdiSubWindowSerializeHead.cpp \
    SAProjectManager.cpp

FORMS += \
    SAValueSelectDialog.ui \
    TestWidget.ui \
    SAFontSetWidget.ui \
    SAAligmentSetWidget.ui \
    SATendencyChartDataSelectDialog.ui

#Chart2D
include($$PWD/Chart2D/Chart2D.pri)
#sa property item
include($$PWD/SAPropertyItem/SAPropertyItem.pri)
#sa chart support
include($$PWD/Private/Private.pri)
#menu
include($$PWD/menu/menu.pri)
#figure set
include($$PWD/figureSet/figureSet.pri)
RESOURCES += \
    icon.qrc
