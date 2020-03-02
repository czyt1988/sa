#-------------------------------------------------
#
# Project created by QtCreator 2016-11-17T17:24:29
# 文本文件导入插件
# 此插件必须加载SAPluginInterface.h，所有插件的类都需要重载SAPluginInterface.h里定义的接口
# 此插件有两种，一种是静态插,一种是动态插,静态插件需在pro加上CONFIG += plugin，静态插件会放置在sa的plugin的dataImport文件夹下的static目录
# 动态插件不需要加上CONFIG += plugin，只需编译出dll即可，放置在sa的plugin的dataImport文件夹下
# 对于动态plugin,需要有导出函数,dataImport对应SADataImportFactory.h,全局就此一个导出函??用户需要给其添加实??#
#-------------------------------------------------
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE      = lib
CONFIG       += C++11
#CONFIG += plugin
#sa api support
include($$PWD/../../3rdParty/qwt/qwt_set.pri)
include($$PWD/../../signALib/signALib.pri)

#sa signACommonUI support
include($$PWD/../../signACommonUI/signACommonUIWith3thLib.pri)

DEFINES += SA_COMMON_PLUGIN_LIB_MAKE#创建插件必须添加此宏
#DEFINES += SA_QT_PLUGIN_MAKE#创建Qt插件必须添加此宏


TARGET        = textImport

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../../bin_qt$$[QT_VERSION]_debug/plugin
}else {
    DESTDIR = $$PWD/../../bin_qt$$[QT_VERSION]_release/plugin
}
HEADERS += \
    SATextDataImport.h \
    TextFileImportDialog.h \
    SATextParser.h \
    SACsvParser.h \
    TextImportConfig.h \
    CheckedHeaderView.h \
    SADataImportFactory.h \
    textImportGlobal.h \
    SACfxCsv2DDataParser.h \
    CfxCsvDataImportConfig.h \
    CFX2DCsvImportDialog.h

SOURCES += \
    SATextDataImport.cpp \
    TextFileImportDialog.cpp \
    SATextParser.cpp \
    SACsvParser.cpp \
    TextImportConfig.cpp \
    CheckedHeaderView.cpp \
    SACfxCsv2DDataParser.cpp \
    CfxCsvDataImportConfig.cpp \
    CFX2DCsvImportDialog.cpp

FORMS += \
    TextFileImportDialog.ui \
    CFX2DCsvImportDialog.ui

RESOURCES += \
    icon.qrc

