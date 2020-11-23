#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T16:30:19
#
#-------------------------------------------------
message("--------------unit test:SA protocol--------------------------")
QT       += network xml testlib gui widgets

TARGET = tst_SignAProtocolTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/
}
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
HEADERS += \
    tst_SignAProtocol.h \
    tst_SAXMLProtocolParser.h

SOURCES += \
    tst_SAVariantCaster.cpp \
    tst_SignAProtocol.cpp \
    tst_SAXMLProtocolParser.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

#sa protocol support
include($$PWD/signAProtocol.pri)



