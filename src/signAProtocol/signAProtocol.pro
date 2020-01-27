#-------------------------------------------------
#
# Project created by czy 2019-08-20
# sa protocol lib
#
#-------------------------------------------------

message("")
message("--------------SA protocol about lib--------------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])

QT += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = signAProtocol
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/libs
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/libs
}
CONFIG += c++11

include($$PWD/../czy/czy_math.pri)

DEFINES += SA_PROTOCOL_MAKE

HEADERS += \
    SAProtocolGlobal.h \
    SAVariantCaster.h \
    SAXMLTagDefined.h \
    SAXMLReadHelper.h \
    SAGlobalConfig.h \
    SAGlobalConfigDefine.h \
    SAConfigXMLReadWriter.h \
    SAAbstractProtocolMaker.h \
    SAXMLProtocolMaker.h \
    SAProtocolHeader.h \
    SACRC.h \
    SAXMLProtocolReadWriter.h \
    SAXMLProtocolParser.h \
    SAAbstractProtocolParser.h

SOURCES += \
    SAVariantCaster.cpp \
    SAXMLReadHelper.cpp \
    SAGlobalConfig.cpp \
    SAConfigXMLReadWriter.cpp \
    SAAbstractProtocolMaker.cpp \
    SAXMLProtocolMaker.cpp \
    SAAbstractProtocolParse.cpp \
    SAProtocolHeader.cpp \
    SACRC.cpp \
    SAXMLProtocolReadWriter.cpp \
    SAXMLProtocolParser.cpp

OTHER_FILES += readme.md
