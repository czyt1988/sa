
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
        msvc:LIBS += -L$${QWT_ROOT}/lib/msvc/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
        mingw:LIBS += -L$${QWT_ROOT}/lib/mingw32/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
    }
    unix{
        LIBS += -L$${QWT_ROOT}/lib/unix/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
    }
}
else {
    #LIBS      += -L$${QWT_ROOT}/lib
    win32{
        msvc:LIBS += -L$${QWT_ROOT}/lib/msvc/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
        mingw:LIBS += -L$${QWT_ROOT}/lib/mingw32/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
    }
    unix{
        LIBS += -L$${QWT_ROOT}/lib/unix/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
    }
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
