#-------------------------------------------------
#
# Project created by czy 2019-08-20
# sa protocol lib
#
#-------------------------------------------------


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


DEFINES += SA_PROTOCOL_MAKE

HEADERS += \
    SAProtocolGlobal.h \
    SAVariantCaster.h \
    SAXMLTagDefined.h \
    SAGlobalConfig.h \
    SAGlobalConfigDefine.h \
    SAProtocolHeader.h \
    SACRC.h \
    SAXMLProtocolParser.h \
    SAAbstractProtocolParser.h \
    SAXMLConfigParser.h

SOURCES += \
    SAVariantCaster.cpp \
    SAGlobalConfig.cpp \
    SAAbstractProtocolParse.cpp \
    SAProtocolHeader.cpp \
    SACRC.cpp \
    SAXMLProtocolParser.cpp \
    SAXMLConfigParser.cpp

OTHER_FILES += readme.md
