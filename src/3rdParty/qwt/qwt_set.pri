
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

TEMPLATE += fakelib
QWT_LIBNAME = $$qtLibraryTarget(qwt)
TEMPLATE -= fakelib

contains(QT_ARCH, i386) {
    win32{
        msvc:QWT_LIB_DIR = $${QWT_ROOT}/lib/msvc/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
        mingw:QWT_LIB_DIR = $${QWT_ROOT}/lib/mingw32/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
    }
    unix{
        QWT_LIB_DIR = $${QWT_ROOT}/lib/unix/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
    }
}else{
    win32{
        msvc:QWT_LIB_DIR = $${QWT_ROOT}/lib/msvc_64/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
        mingw:QWT_LIB_DIR = $${QWT_ROOT}/lib/mingw32_64/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
    }
    unix{
        QWT_LIB_DIR = $${QWT_ROOT}/lib/unix/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
    }
}
message("qwt lib dir")
message($$QWT_LIB_DIR)
message($$QWT_LIBNAME)

contains(QWT_CONFIG, QwtFramework) {
#添加平台判断
    LIBS += -L$$QWT_LIB_DIR -l$$QWT_LIBNAME
}
else {
    LIBS += -L$$QWT_LIB_DIR -l$$QWT_LIBNAME
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
