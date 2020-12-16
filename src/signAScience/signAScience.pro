#-------------------------------------------------
#
# Project created by czy 2020-3-1
# sa science lib
#
#-------------------------------------------------

message("------------signAScience SA科学计算相关封装-------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" #忽略warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
QT += core gui
TARGET = signAScience
TEMPLATE = lib
CONFIG += c++11
include(../sa_common.pri)
DESTDIR = $$SA_LIBDIR

DEFINES += SASCIENCE_MAKELIB
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



HEADERS += \
    SAInterpolation.h \
    SAPolyFit.h \
    SAScienceGlobal.h\
    SADsp.h \
    SAMath.h \
    SAScienceDefine.h \
    SASmooth.h

SOURCES += \
    SADsp.cpp \
    SAInterpolation.cpp \
    SAPolyFit.cpp \
    SASmooth.cpp


#the gsl lib support
include($$PWD/../3rdParty/gsl/gsl.pri)

#the fftw lib support
include($$PWD/../3rdParty/fftw/fftw.pri)

