#-------------------------------------------------
#
# Project created by QtCreator 2016-11-17T17:24:29
# 文本文件导入插件
# 此插件必须加载SAPluginInterface.h，所有插件的类都需要重载SAPluginInterface.h里定义的接口
# 此插件有两种，一种是静态插,一种是动态插,静态插件需在pro加上CONFIG += plugin，静态插件会放置在sa的plugin的dataImport文件夹下的static目录
# 动态插件不需要加上CONFIG += plugin，只需编译出dll即可，放置在sa的plugin文件夹下
# 对于动态plugin,需要有导出函数,dataImport对应SADataImportFactory.h,全局就此一个导出函??用户需要给其添加实??#
#-------------------------------------------------

message("------------插件--FunPlugin函数集合-------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" #忽略warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE      = lib
CONFIG       += C++11

#sa api support
include($$PWD/../../3rdParty/qwt/qwt_set.pri)
include($$PWD/../../signALib/signALib.pri)
include($$PWD/../../signACoreFun/signACoreFun.pri)
include($$PWD/../../signACommonUI/signACommonUIWith3thLib.pri)
include($$PWD/../../3rdParty/qtpropertybrowser/propertybrowser.pri)
#sa chart support
#include($$PWD/../../signAChart/signAChart.pri)

#czy qt api support

include($$PWD/../../signAUtil/signAUtil.pri)
include($$PWD/../../signAScience/signAScience.pri)
DEFINES += SA_COMMON_PLUGIN_LIB_MAKE#创建插件必须添加此宏


TARGET        = FunPlugin

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../../bin_qt$$[QT_VERSION]_debug/plugin
}else {
    DESTDIR = $$PWD/../../bin_qt$$[QT_VERSION]_release/plugin
}

HEADERS += \
    SAFunPlugin.h \
    Resource.h \
    SATimeFrequencyAnalysis.h \
    FunDsp.h \
    FunDataPreprocessing.h \
    FunNum.h \
    FunFit.h \
    FitParamSetDialog.h \
    ui_opt.h

SOURCES += \
    SAFunPlugin.cpp \
    SATimeFrequencyAnalysis.cpp \
    FunDsp.cpp \
    FunDataPreprocessing.cpp \
    FunNum.cpp \
    FunFit.cpp \
    FitParamSetDialog.cpp \
    ui_opt.cpp

include($$PWD/Dialog/Dialog.pri)

RESOURCES += \
    icon.qrc

FORMS += \
    SATimeFrequencyAnalysis.ui \
    FitParamSetDialog.ui
