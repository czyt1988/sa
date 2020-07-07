exists(config.pri):infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtpropertybrowser-uselib

TEMPLATE += fakelib
QTPROPERTYBROWSER_LIBNAME = $$qtLibraryTarget(QtPropertyBrowser)
TEMPLATE -= fakelib

CONFIG(debug, debug|release){
QTPROPERTYBROWSER_LIBDIR = $$PWD/lib/bin_qt$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_debug
}else{
QTPROPERTYBROWSER_LIBDIR = $$PWD/lib/bin_qt$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_release
}
unix:qtpropertybrowser-uselib:!qtpropertybrowser-buildlib:QMAKE_RPATHDIR += $$QTPROPERTYBROWSER_LIBDIR
