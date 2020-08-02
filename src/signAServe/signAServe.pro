#-------------------------------------------------
#
# Project created by czy 2019-10-14
# sa rpc lib by qt bin stream
#
#-------------------------------------------------

message("--------------signAServe SA服务封装--------------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" #忽略warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
QT += core
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = signAServe
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/libs
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/libs
}
DEFINES += SASERVE_MAKE \
           SA_SERVE_DEBUG_PRINT

HEADERS += \
    $$PWD/../global/SAGlobals.h \
    SAServeGlobal.h \
    SAAbstractServe.h \
    SATcpDataProcessSocket.h \
    SATcpSocket.h \
    SATcpServe.h \
    SAServeShareMemory.h \
    SATcpClient.h \
    SAServerDefine.h \
    SAServeHandleFun.h



SOURCES += \
    SAAbstractServe.cpp \
    SATcpDataProcessSocket.cpp \
    SATcpSocket.cpp \
    SATcpServe.cpp \
    SAServeShareMemory.cpp \
    SATcpClient.cpp \
    SAServeHandleFun.cpp

#sa lib
include($$PWD/../signALib/signALib.pri)
#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)

OTHER_FILES += readme.md
