win32{
    contains(DEFINES, WIN64) {
        message("setup fftw in win64")
        HEADERS += \
            $$PWD/fftw64/fftw3.h
        LIBS += -L$$PWD\fftw64 -llibfftw3-3
        INCLUDEPATH += $$PWD\fftw64
    } else {
        message("setup fftw in win32")
        HEADERS += \
            $$PWD/fftw32/fftw3.h
        LIBS += -L$$PWD\fftw32 -llibfftw3-3
        INCLUDEPATH += $$PWD\fftw32
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