INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
# user need add 
#include($$PWD/../3rdParty/qwt/qwt_set.pri)
# user need add 
#include($$PWD/../3rdParty/qtpropertybrowser/src/qtpropertybrowser.pri)#propertybrowser support
DEFINES += USE_QWT
CONFIG += qwt
QWT_CONFIG  += QwtDll
include(../sa_common.pri)
LIBS += -L$$SA_LIBDIR -lsignAChart

