INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/SALocalServerDefine.h \
    $$PWD/SALocalServeBaseHeader.h \
    $$PWD/SALocalServeReader.h \
    $$PWD/SALocalServeWriter.h \
    $$PWD/SALocalServeProtocol.h

SOURCES += \
    $$PWD/SALocalServeBaseHeader.cpp \
    $$PWD/SALocalServeReader.cpp \
    $$PWD/SALocalServeWriter.cpp \
    $$PWD/SALocalServeProtocol.cpp
