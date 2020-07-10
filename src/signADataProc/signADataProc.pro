#-------------------------------------------------
#
# Project created by QtCreator 2017-06-16 13:23:43
#
# 关于此服务端用哪个通讯方式进行了很多试探，最开始的有名管道，到rpc，最后又回归到tcp/ip协议，发现
# 如果要做到平台无关和界面分离，tcp/ip协议是最好的选择，甚至后面可以抛弃客户端，开发一个b/s端
# 因此，客户端和服务端的交互，使用tcp/ip协议，内容自我封装为json进行解析
#
#-------------------------------------------------
message("--------------SA Data Process Serve 进程处理服务--------------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" #忽略warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
win32: QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
win32: QMAKE_CFLAGS_RELEASE -= -Zc:strictStrings
win32: QMAKE_CFLAGS -= -Zc:strictStrings
win32: QMAKE_CXXFLAGS -= -Zc:strictStrings
CONFIG+=force_debug_info separate_debug_info


QT += core gui
QT += concurrent
QT += network
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


OTHER_FILES += readme.md

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/
}

TARGET = signADataProc
TEMPLATE = app
CONFIG += c++11
INCLUDEPATH += $$PWD


SOURCES += main.cpp \
    SADataProcFunctions.cpp \
    SADataProcHandle.cpp \
    SADataProcServe.cpp \
    SAMiniDump.cpp \
    runnable/SADataStatisticRunable.cpp

HEADERS += \
    SADataProcFunctions.h \
    SADataProcHandle.h \
    SADataProcServe.h \
    SAMiniDump.h \
    runnable/SADataStatisticRunable.h
    

#sa api support
#{
#sa api support
include($$PWD/../signAUtil/signAUtil.pri)
include($$PWD/../signALib/signALib.pri)
include($$PWD/../signAProtocol/signAProtocol.pri)
include($$PWD/../signAServe/signAServe.pri)
include($$PWD/../signAScience/signAScience.pri)
#}



# 给dump文件生成用
win32{
    LIBS += -luser32
}



