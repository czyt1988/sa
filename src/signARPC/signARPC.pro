#-------------------------------------------------
#
# Project created by czy 2019-10-14
# sa rpc lib by qt bin stream
#
#-------------------------------------------------
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
    SALocalServeBaseHeader.h \
    SALocalServerDefine.h \
    SALocalServeSocketOpt.h \
    SAProcedure.h \
    SARPCGlobal.h \
    SACRC.h \
    3rdParty/CRC.h



SOURCES += \
    SAAbstractRPCServe.cpp \
    SALocalServeBaseHeader.cpp \
    SALocalServeSocketOpt.cpp \
    SAProcedure.cpp \
    SACRC.cpp

#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)
include($$PWD/../czy/czy.pri)

