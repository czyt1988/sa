exists(config.pri):infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtpropertybrowser-uselib
TEMPLATE += fakelib
QTPROPERTYBROWSER_LIBNAME = $$qtLibraryTarget(QtSolutions_PropertyBrowser-head)
TEMPLATE -= fakelib
QTPROPERTYBROWSER_LIBDIR = $$PWD/lib/bin_qt$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}
unix:qtpropertybrowser-uselib:!qtpropertybrowser-buildlib:QMAKE_RPATHDIR += $$QTPROPERTYBROWSER_LIBDIR
