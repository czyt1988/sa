INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD


CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_debug/libs/ -lsignAProtocol
}else {
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_release/libs/ -lsignAProtocol
}
