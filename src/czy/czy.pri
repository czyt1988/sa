INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD\qt_extend
DEPENDPATH += $$PWD

include($$PWD/gsl/gsl.pri)#the gsl lib support

QT += core gui

HEADERS += \
        $$PWD/czy.h \
        $$PWD/czyMath_DSP.h \
        $$PWD/czyMath.h \
        $$PWD/czyMath_Interpolation.h \
        $$PWD/czyMath_Fit.h \
        $$PWD/czyMath_Smooth.h


SOURCES +=  \
        $$PWD/czyMath_DSP.cpp\
        $$PWD/czyMath_Smooth.cpp

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
