#
# 请把此句添加到需要引用的pro文件中
# include($$[QT_INSTALL_PREFIX]/../Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri)
#
#
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/../3rdParty/qti
DEPENDPATH += $$PWD/../3rdParty/qti
INCLUDEPATH += $$PWD/Chart2D
DEPENDPATH += $$PWD/Chart2D

LIBS += -L$$PWD/../bin/libs/ -lsignACommonUI
# user need add 
#include($$PWD/../3rdParty/qwt/qwt_set.pri)
# user need add 
#include($$[QT_INSTALL_PREFIX]/../Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri)

