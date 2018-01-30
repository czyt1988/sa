INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD\qt_extend
DEPENDPATH += $$PWD

QT += core gui

HEADERS += \
		$$PWD/czy.h \
        $$PWD/czyQtApp.h \
        $$PWD/czyQArrayEx.h \
        $$PWD/czyQtUI.h


SOURCES +=  \
        $$PWD/czyQtUI.cpp \
        $$PWD/czyQtApp.cpp
