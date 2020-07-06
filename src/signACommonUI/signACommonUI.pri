INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/../3rdParty/qti
DEPENDPATH += $$PWD/../3rdParty/qti
INCLUDEPATH += $$PWD/Chart2D
DEPENDPATH += $$PWD/Chart2D

CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_debug/libs/ -lsignACommonUI
}else {
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_release/libs/ -lsignACommonUI
}

