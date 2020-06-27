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
DEFINES += SASERVE_MAKE \
           SA_SERVE_DEBUG_PRINT

HEADERS += \
    /../global/SAGlobals.h \
    SAServeGlobal.h \
    SAAbstractServe.h \
    3rdParty/CRC.h \
    SASession.h \
    SATcpSocket.h \
    SATcpServe.h \
    SAServeShareMemory.h \
    SATcpClient.h \
    SAServerDefine.h \
    SATcpDataProcessClient.h \
    SAServeHandleFun.h \
    SATcpThreadSocket.h \
    ../global/SAGlobals.h



SOURCES += \
    SAAbstractServe.cpp \
    SASession.cpp \
    SATcpSocket.cpp \
    SATcpServe.cpp \
    SAServeShareMemory.cpp \
    SATcpClient.cpp \
    SATcpDataProcessClient.cpp \
    SAServeHandleFun.cpp \
    SATcpThreadSocket.cpp

#sa lib
include($$PWD/../signALib/signALib.pri)
#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)

OTHER_FILES += readme.md
