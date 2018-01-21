INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/SALocalServerDefine.h \
    $$PWD/SALocalServeBaseHeader.h \
    $$PWD/SALocalServeFigureItemProcessHeader.h \
    $$PWD/SALocalServeReader.h \
    $$PWD/SALocalServeWriter.h \
    $$PWD/SALocalServeBaseProtocol.h

SOURCES += \
    $$PWD/SALocalServeBaseHeader.cpp \
    $$PWD/SALocalServeFigureItemProcessHeader.cpp \
    $$PWD/SALocalServeReader.cpp \
    $$PWD/SALocalServeWriter.cpp \
    $$PWD/SALocalServeBaseProtocol.cpp
