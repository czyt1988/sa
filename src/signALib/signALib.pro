#-------------------------------------------------
#
# Project created by czy 2016-10-14
# sa api lib
#
#-------------------------------------------------
QT += core gui xml
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = signALib
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug/libs
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release/libs
}

CONFIG += c++11

HEADERS += \
    /../global/SAGlobals.h \
    SAData.h \
    SADataPackage.h \
    SAItem.h \
    SALibGlobal.h \
    SAValueManager.h \
    SAValueManagerModel.h \
    SARandColorMaker.h \
    SACsvStream.h \
    SALog.h \ 
    SAPluginInterface.h \
    SATextReadWriter.h \
    SADataHeader.h \
    SALibResourDefine.h \
    SADataFeatureItem.h \
    SAValueManagerMimeData.h \
    SAValueManagerListModel.h \
    SAHashTable.h \
    SATree.h \
    SATreeModel.h


SOURCES += \
    SAData.cpp \
    SADataPackage.cpp \
    SAItem.cpp \
    SAValueManager.cpp \
    SAValueManagerModel.cpp \
    SARandColorMaker.cpp \
    SACsvStream.cpp \
    SALog.cpp \
    SATextReadWriter.cpp \
    SADataHeader.cpp \
    SADataFeatureItem.cpp \
    SAValueManagerMimeData.cpp \
    SAValueManagerListModel.cpp \
    SATree.cpp \
    SATreeModel.cpp

#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)
#
include($$PWD/Private/Private.pri)
include($$PWD/SABaseValueType/SABaseValueType.pri)

include($$PWD/../czy/czy.pri)
include($$PWD/../3rdParty/qwt/qwt_set.pri)
DEFINES += SALIB_MAKE #定义此宏将构建库
#RESOURCES += \
#    ico.qrc

RESOURCES += \
    icon.qrc
