#-------------------------------------------------
#
# Project created by czy 2019-10-14
# sa rpc lib by qt bin stream
#
#-------------------------------------------------

message("")
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
DEFINES += SASERVE_MAKE #定义此宏将构建库
CONFIG += c++11

HEADERS += \
    /../global/SAGlobals.h \
    SAServeGlobal.h \
    SAAbstractServe.h \
    SALocalServeSocketOpt.h \
    SAProcedure.h \
    3rdParty/CRC.h \
    SATcpSocket.h \
    SATcpServe.h \
    SATcpAssignServe.h \
    SAServeShareMemory.h \
    SATcpClient.h \
    SAServerDefine.h



SOURCES += \
    SALocalServeSocketOpt.cpp \
    SAProcedure.cpp \
    SAAbstractServe.cpp \
    SATcpSocket.cpp \
    SATcpServe.cpp \
    SATcpAssignServe.cpp \
    SAServeShareMemory.cpp \
    SATcpClient.cpp

#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)
include($$PWD/../czy/czy.pri)

OTHER_FILES += readme.md
