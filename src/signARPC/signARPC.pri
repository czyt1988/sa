INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD


CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_debug/libs/ -lsignARPC
}else {
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_release/libs/ -lsignARPC
}
