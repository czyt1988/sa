#-------------------------------------------------
#
# Project created by czy 2016-12-21
# sa core fun
#
#-------------------------------------------------

QT += core
TARGET = signACoreFun
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/libs
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/libs
}
CONFIG += c++11

include($$PWD/../czy/czy_math.pri)
#sa api support
include($$PWD/../signALib/signALib.pri)
include($$PWD/../3rdParty/qwt/qwt_set.pri)
DEFINES += SA_CORE_FUN_MAKE

HEADERS += \
    SAAbstractFunctionItem.h \
    SAStandardFunctionItem.h \
    SACoreFunGlobal.h \
    sa_fun_dsp.h \
    sa_fun_num.h \
    sa_fun_core.h \
    sa_fun_core_std.h \
    sa_fun_preproc.h \
    sa_fun_fit.h

SOURCES += \
    SAStandardFunctionItem.cpp \
    sa_fun_dsp.cpp \
    sa_fun_num.cpp \
    sa_fun_core.cpp \
    sa_fun_preproc.cpp \
    sa_fun_fit.cpp
