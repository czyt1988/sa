#-------------------------------------------------
#
# Project created by czy 2016-11-04
# sa chart api lib
#
#-------------------------------------------------
message("------------signACommonUI 通用UI交互封装-------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" #忽略warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
QT += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = signACommonUI
TEMPLATE = lib
CONFIG += dll
include(../sa_common.pri)
DESTDIR = $$SA_LIBDIR
DEPENDPATH += $$SA_LIBDIR


CONFIG += c++11
DEFINES += SA_COMMON_UI_MAKE #make dll
DEFINES += SA_COMMON_UI_USE_DYNAMIC #定义使用dynamic_cast 而不是rtti

#czy
#sa util
include($$PWD/../signAUtil/signAUtil.pri)
#sa api support
include($$PWD/../signALib/signALib.pri)
#sa protocal support
include($$PWD/../signAProtocol/signAProtocol.pri)
#sa chart support
include($$PWD/../signAChart/signAChart.pri)
#sa core fun support
include($$PWD/../signACoreFun/signACoreFun.pri)
#propertybrowser
include($$PWD/../3rdParty/qtpropertybrowser/propertyBrowser.pri)
#QWT
include($$PWD/../3rdParty/qwt/qwt_set.pri)
#color picker support
include($$PWD/../3rdParty/qtcolorpicker/qtcolorpicker.pri)
#CTK
include($$PWD/../3rdParty/ctk/ctk.pri)
#QTI
include($$PWD/../3rdParty/qti/qti.pri)

OTHER_FILES += readme.md


#Chart2D
include($$PWD/Chart2D/Chart2D.pri)

HEADERS += \
    SACommonUIGlobal.h \
    SAFigureWindowChartTableModel.h \
    SAQwtSymbolComboBox.h \
    SACollapsibleGroupBox.h \
    SAVCollapsibleGroupBox.h \
    SAUIReflection.h \
    SAValueSelectDialog.h \
    SAUIInterface.h \
    SAMdiSubWindow.h \
    SAPenSetWidget.h \
    SAColorPickerButton.h \
    SAPenStyleComboBox.h \
    SAFontSetWidget.h \
    SAAligmentSetWidget.h \
    SAValueManagerTreeView.h \
    SATendencyChartDataSelectDialog.h \
    SAMdiSubWindowSerializeHead.h \
    SAProjectManager.h \
    SATimeFormatSetWidget.h \
    SATableView.h \
    SAPropertySetDialog.h \
    SAPropertySetWidget.h \
    SAUIHelper.h \
    SADataTableModel.h \
    SAPopupWidget.h \
    SAWaitCursor.h \
    SAValueTableOptCommands.h \
    SAPlotDataModel.h \
    SAGUIGlobalConfig.h \
    SAFigureSetWidget.h \
    SAChartSetWidget.h \
    SAMainWindow.h

SOURCES += \
    SAFigureWindowChartTableModel.cpp \
    SAUIReflection.cpp \
    SAQwtSymbolComboBox.cpp \
    SACollapsibleGroupBox.cpp \
    SAVCollapsibleGroupBox.cpp \
    SAValueSelectDialog.cpp \
    SAUIInterface.cpp \
    SAMdiSubWindow.cpp \
    SAPenSetWidget.cpp \
    SAColorPickerButton.cpp \
    SAPenStyleComboBox.cpp \
    SAFontSetWidget.cpp \
    SAAligmentSetWidget.cpp \
    SAValueManagerTreeView.cpp \
    SATendencyChartDataSelectDialog.cpp \
    SAMdiSubWindowSerializeHead.cpp \
    SAProjectManager.cpp \
    SATimeFormatSetWidget.cpp \
    SATableView.cpp \
    SAPropertySetDialog.cpp \
    SAPropertySetWidget.cpp \
    SAUIHelper.cpp \
    SADataTableModel.cpp \
    SAPopupWidget.cpp \
    SAWaitCursor.cpp \
    SAValueTableOptCommands.cpp \
    SAPlotDataModel.cpp \
    SAValueTableOptEditValueCommand_impl.cpp \
    SAValueTableOptPasteCommand_impl.cpp \
    SAValueTableOptDeleteCommand_impl.cpp \
    SAValueTableOptInsertCommand_impl.cpp \
    SAGUIGlobalConfig.cpp \
    SAFigureSetWidget.cpp \
    SAChartSetWidget.cpp \
    SAMainWindow.cpp

FORMS += \
    SAValueSelectDialog.ui \
    SAFontSetWidget.ui \
    SAAligmentSetWidget.ui \
    SATendencyChartDataSelectDialog.ui \
    SATimeFormatSetWidget.ui


#sa property item
include($$PWD/SAPropertyItem/SAPropertyItem.pri)
#sa chart support
include($$PWD/Private/Private.pri)
#figure set
include($$PWD/figureSet/figureSet.pri)
RESOURCES += \
    icon.qrc

