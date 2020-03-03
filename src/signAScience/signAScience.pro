QT -= gui

TEMPLATE = lib
DEFINES += SASCIENCE_MAKELIB

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \ \
    SADsp.cpp \
    SAMath.cpp


HEADERS += \
    SADsp.h \
    SAMath.h \
    SAScienceGlobal.h

include($$PWD/gsl/gsl.pri)#the gsl lib support
win32{
    contains(DEFINES, WIN64) {
        message("win64")
        HEADERS += \
            $$PWD/fftw64/fftw3.h
        LIBS += -L$$PWD\fftw64 -llibfftw3-3
        INCLUDEPATH += $$PWD\fftw64
    } else {
        message("win32")
        HEADERS += \
            $$PWD/fftw/fftw3.h
        LIBS += -L$$PWD\fftw -llibfftw3-3
        INCLUDEPATH += $$PWD\fftw
    }
}#unix下直接安装部署fftw
unix{
    INCLUDEPATH += usr/local/include
    DEPENDPATH += usr/local/lib
    LIBS += -L/usr/local/lib -lfftw3
}
# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
