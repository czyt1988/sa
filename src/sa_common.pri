CONFIG(debug, debug|release){
    contains(QT_ARCH, i386) {
        SA_BIN_DIR = $$PWD/bin_qt$$[QT_VERSION]_debug
    }else {
        SA_BIN_DIR = $$PWD/bin_qt$$[QT_VERSION]_debug_64
    }
}else{
    contains(QT_ARCH, i386) {
        SA_BIN_DIR = $$PWD/bin_qt$$[QT_VERSION]_release
    }else {
        SA_BIN_DIR = $$PWD/bin_qt$$[QT_VERSION]_release_64
    }
}
SA_LIBDIR = $$SA_BIN_DIR/libs
SA_PLUGIN_LIBDIR = $$SA_BIN_DIR/plugin
