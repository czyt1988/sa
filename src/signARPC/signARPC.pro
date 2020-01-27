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
TARGET = signARPC
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/libs
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/libs
}
DEFINES += SARPC_MAKE #定义此宏将构建库
CONFIG += c++11

HEADERS += \
    /../global/SAGlobals.h \
    SARPCGlobal.h \
    SAAbstractRPCServe.h \
    SALocalServerDefine.h \
    SALocalServeSocketOpt.h \
    SAProcedure.h \
    SARPCGlobal.h \
    3rdParty/CRC.h \
    SAXMLRPCServe.h



SOURCES += \
    SAAbstractRPCServe.cpp \
    SALocalServeSocketOpt.cpp \
    SAProcedure.cpp \
    SAXMLRPCServe.cpp

#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)
include($$PWD/../czy/czy.pri)

