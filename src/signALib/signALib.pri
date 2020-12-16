INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD/SABaseValueType
DEPENDPATH += $$PWD/SABaseValueType

INCLUDEPATH += $$PWD/LocalServer
DEPENDPATH += $$PWD/LocalServer

include(../sa_common.pri)
LIBS += -L$$SA_LIBDIR -lsignALib
