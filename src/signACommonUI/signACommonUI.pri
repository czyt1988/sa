INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -L$$PWD/../bin/libs/ -lsignACommonUI
# user need add 
#include($$PWD/../3rdParty/qwt/qwt_set.pri)
# user need add 
#include($$PWD/../3rdParty/qtpropertybrowser/src/qtpropertybrowser.pri)#propertybrowser support
