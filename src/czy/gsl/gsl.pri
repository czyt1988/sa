win32{#win32下需要gnu32
    DEFINES += GSL_DLL#此句不加将会遇到一些连接错误
    INCLUDEPATH += $$PWD\include
    DEPENDPATH += $$PWD
    LIBS += -L$$PWD\lib -llibgsl
    LIBS += -L$$PWD\lib -llibgslcblas
}
unix{
    #下载gsl源码
    #cd gsl-1.x
    #sudo ./configure
    #sudo make
    #sudo make install
    INCLUDEPATH += usr/local/include/gsl
    DEPENDPATH += usr/local/lib
    LIBS += -L/usr/local/lib -lgsl
    LIBS += -L/usr/local/lib -lgslcblas
}
