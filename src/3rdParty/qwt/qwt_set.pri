
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src
#qwt set
QWT_ROOT = $$PWD


include( $${QWT_ROOT}/qwtfunctions.pri )
CONFIG += qwt
QWT_CONFIG  += QwtDll

INCLUDEPATH += $${QWT_ROOT}/src
DEPENDPATH  += $${QWT_ROOT}/src

contains(QWT_CONFIG, QwtFramework) {
#添加平台判断
    message("qwt:contain QwtFramework")
    win32{
        msvc:LIBS += -L$${QWT_ROOT}/lib/x86/msvc
        mingw:LIBS += -L$${QWT_ROOT}/lib/x86/mingw32
    }
    unix{
        LIBS += -L$${QWT_ROOT}/lib/unix
    }
}
else {
    message("qwt:not contain QwtFramework")
    #LIBS      += -L$${QWT_ROOT}/lib
    win32{
        msvc:LIBS += -L$${QWT_ROOT}/lib/x86/msvc
        mingw:LIBS += -L$${QWT_ROOT}/lib/x86/mingw32
    }
    unix{
        LIBS += -L$${QWT_ROOT}/lib/unix
    }
}
qwtAddLibrary(qwt)
message(qwt libs : $${LIBS})
contains(QWT_CONFIG, QwtOpenGL ) {

    QT += opengl
}
else {

    DEFINES += QWT_NO_OPENGL
}
contains(QWT_CONFIG, QwtSvg) {

    QT += svg
}
else {

    DEFINES += QWT_NO_SVG
}
win32 {
    contains(QWT_CONFIG, QwtDll) {
        DEFINES    += QT_DLL QWT_DLL
    }
}
#end qwt set
