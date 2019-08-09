INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD\qt_extend
DEPENDPATH += $$PWD

include($$PWD/gsl/gsl.pri)#the gsl lib support

QT += core gui

HEADERS += \
        $$PWD/czy.h \
        $$PWD/czyQtApp.h \
        $$PWD/czyQtUI.h \
        $$PWD/czyMath_DSP.h \
        $$PWD/czyMath.h \
        $$PWD/czyMath_Interpolation.h \
        $$PWD/czyMath_Fit.h \
        $$PWD/czySA.h \
        $$PWD/czyAlgorithm.h \
        $$PWD/czyMath_Smooth.h \
    $$PWD/qt_extend/czyQtPointAlgorithm.h \
    $$PWD/czyArray.h \
    $$PWD/czyQArray.h


SOURCES +=  \
        $$PWD/czyMath_DSP.cpp\
        $$PWD/czyQtUI.cpp \
        $$PWD/czySA.cpp\
        $$PWD/czyAlgorithm.cpp \
        $$PWD/czyMath_Smooth.cpp \
        $$PWD/czyQtApp.cpp

win32{
    HEADERS += \
        $$PWD/fftw/fftw3.h
    LIBS += -L$$PWD\fftw -llibfftw3-3
    INCLUDEPATH += $$PWD\fftw
}#unix下直接安装部署fftw
unix{
    INCLUDEPATH += usr/local/include
    DEPENDPATH += usr/local/lib
    LIBS += -L/usr/local/lib -lfftw3
}
