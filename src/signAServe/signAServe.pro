#-------------------------------------------------
#
# Project created by czy 2019-10-14
# sa rpc lib by qt bin stream
#
#-------------------------------------------------

message("--------------SA Serve about lib--------------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])

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
DEFINES += SASERVE_MAKE \#定义此宏将构建库
           SA_SERVE_DEBUG_PRINT #debug时打印
CONFIG += c++11

HEADERS += \
    /../global/SAGlobals.h \
    SAServeGlobal.h \
    SAAbstractServe.h \
    3rdParty/CRC.h \
    SATcpSocket.h \
    SATcpServe.h \
    SATcpAssignServe.h \
    SAServeShareMemory.h \
    SATcpClient.h \
    SAServerDefine.h \
    SATcpDataProcessClient.h \
    SATcpSection.h \
    SAServeHandleFun.h



SOURCES += \
    SAAbstractServe.cpp \
    SATcpSocket.cpp \
    SATcpServe.cpp \
    SATcpAssignServe.cpp \
    SAServeShareMemory.cpp \
    SATcpClient.cpp \
    SATcpDataProcessClient.cpp \
    SATcpSection.cpp \
    SAServeHandleFun.cpp

#sa lib
include($$PWD/../signALib/signALib.pri)
#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)

OTHER_FILES += readme.md
