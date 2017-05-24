
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
    win32{
        msvc:LIBS += -F$${QWT_ROOT}/lib/x86/msvc
        mingw:LIBS += -F$${QWT_ROOT}/lib/x86/mingw32
    }
    unix{
        LIBS += -F$${QWT_ROOT}/lib/unix
    }
}
else {
    LIBS      += -L$${QWT_ROOT}/lib
}
qwtAddLibrary(qwt)

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
