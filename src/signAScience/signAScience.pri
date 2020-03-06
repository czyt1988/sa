INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
HEADERS += \
    $$PWD/SAMath.h 

CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_debug/libs/ -lsignAScience
}else {
    LIBS += -L$$PWD/../bin_qt$$[QT_VERSION]_release/libs/ -lsignAScience
}