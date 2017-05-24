INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD\qt_extend
DEPENDPATH += $$PWD

QT += core gui

HEADERS += \
		$$PWD/czy.h \
        $$PWD/czyQtApp.h \
        $$PWD/czyQArrayEx.h \
        $$PWD/czyQtUI.h \
        $$PWD/qt_extend/QEmail.h


SOURCES +=  \
        $$PWD/czyQtUI.cpp \
        $$PWD/qt_extend/QEmail.cpp \
        $$PWD/czyQtApp.cpp
