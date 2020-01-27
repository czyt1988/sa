#-------------------------------------------------
#
# Project created by czy 2016-12-21
# SA Process Model
#
#-------------------------------------------------

message("")
message("--------------SA Process Model--------------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])

QT += core
TARGET = signAProcess
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/libs
    target.path = $$PWD/../bin_qt$$[QT_VERSION]_debug/
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/libs
    target.path = $$PWD/../bin_qt$$[QT_VERSION]_release/
}


CONFIG += c++11

include($$PWD/../czy/czy_math.pri)

DEFINES += SA_PROCESS_MAKE

HEADERS += \
    SAProcessGlobal.h \
    SAAbstractProcess.h \
    SAPointSeriesStatisticProcess.h \
    SAThreadProcessPool.h

SOURCES += \
    SAAbstractProcess.cpp \
    SAPointSeriesStatisticProcess.cpp \
    SAThreadProcessPool.cpp

