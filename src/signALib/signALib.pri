INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD/SABaseValueType
DEPENDPATH += $$PWD/SABaseValueType

INCLUDEPATH += $$PWD/LocalServer
DEPENDPATH += $$PWD/LocalServer


CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_debug/libs/ -lsignALib
}else {
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_release/libs/ -lsignALib
}
