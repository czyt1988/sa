#-------------------------------------------------
#
# Project created by czy 2016-12-21
# sa core fun
#
#-------------------------------------------------

QT += core
TARGET = signAProcess
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/libs
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/libs
}
CONFIG += c++11

include($$PWD/../czy/czy_math.pri)

DEFINES += SA_PROCESS_MAKE

HEADERS += \
    SAProcessGlobal.h \
    SAAbstractProcess.h \
    SAPointSeriesStatisticProcess.h

SOURCES += \
    SAAbstractProcess.cpp \
    SAPointSeriesStatisticProcess.cpp

