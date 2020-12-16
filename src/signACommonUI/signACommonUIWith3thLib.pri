INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/Chart2D
DEPENDPATH += $$PWD/Chart2D
include(../sa_common.pri)
LIBS += -L$$SA_LIBDIR -lsignACommonUI

include($$PWD/../signAChart/signAChart.pri)

#qwt
include($$PWD/../3rdParty/qwt/qwt_set.pri)

#propertybrowser
include($$PWD/../3rdParty/qtpropertybrowser/propertyBrowser.pri)
