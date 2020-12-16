INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
HEADERS += \
    $$PWD/SAMath.h 
include(../sa_common.pri)
LIBS += -L$$SA_LIBDIR -lsignAScience

