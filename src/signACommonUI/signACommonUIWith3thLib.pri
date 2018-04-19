INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/Chart2D
DEPENDPATH += $$PWD/Chart2D

LIBS += -L$$PWD/../bin/libs/ -lsignACommonUI
#
include($$PWD/../signAChart/signAChart.pri)

# user need add 
include($$PWD/../3rdParty/qwt/qwt_set.pri)

# 需要在pro中添加
#include($$[QT_INSTALL_PREFIX]/../Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri)
