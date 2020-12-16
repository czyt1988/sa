#-------------------------------------------------
#
# Project created by czy 2016-12-21
# sa core fun
#
#-------------------------------------------------
message("------------signACoreFun 核心函数封装-------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" #忽略warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
QT += core
TARGET = signACoreFun
TEMPLATE = lib

include(../sa_common.pri)
DESTDIR = $$SA_LIBDIR

CONFIG += c++11

#常用程序算法封装
include($$PWD/../signAUtil/signAUtil.pri)
#科学计算相关库
include($$PWD/../signAScience/signAScience.pri)
#sa api support
include($$PWD/../signALib/signALib.pri)
include($$PWD/../3rdParty/qwt/qwt_set.pri)
DEFINES += SA_CORE_FUN_MAKE

HEADERS += \
    SAAbstractFunctionItem.h \
    SAStandardFunctionItem.h \
    SACoreFunGlobal.h \
    sa_fun_dsp.h \
    sa_fun_num.h \
    sa_fun_core.h \
    sa_fun_core_std.h \
    sa_fun_preproc.h \
    sa_fun_fit.h

SOURCES += \
    SAStandardFunctionItem.cpp \
    sa_fun_dsp.cpp \
    sa_fun_num.cpp \
    sa_fun_core.cpp \
    sa_fun_preproc.cpp \
    sa_fun_fit.cpp
