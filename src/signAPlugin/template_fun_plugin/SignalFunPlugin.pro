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

#sa api support
include($$PWD/../../3rdParty/qwt/qwt_set.pri)
include($$PWD/../../signALib/signALib.pri)

DEFINES += SA_COMMON_PLUGIN_LIB_MAKE#创建插件必须添加此宏

#czy qt api support
include($$PWD/../../czy/czy_qt.pri)

TARGET        = signalFun
DESTDIR = ../../bin/plugin

HEADERS += \
    SignalFunPlugin.h 

SOURCES += \
    SignalFunPlugin.cpp 


