win32{
    INCLUDEPATH += $$PWD
    DEPENDPATH += $$PWD
    mingw:{
        message("use mingw")
       # win32: LIBS += -L"D:/Program Files (x86)/Microsoft Visual Studio 10.0/VC/lib" -llibcmt -loldnames -luuid
    }
    HEADERS +=\
        $$PWD/nilibddc.h\
        $$PWD/QTDMFile.h \
        $$PWD/QTDMGroup.h \
        $$PWD/QTDMChannel.h \
        $$PWD/QTDMChannelTableModel.h

    SOURCES +=\
        $$PWD/QTDMFile.cpp \
        $$PWD/QTDMGroup.cpp \
        $$PWD/QTDMChannel.cpp \
        $$PWD/QTDMChannelTableModel.cpp

    LIBS += -L$$PWD -lnilibddc
}
