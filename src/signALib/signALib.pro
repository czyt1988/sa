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
DESTDIR = $$PWD/../bin/libs
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
    SAGlobalConfig.h \
    SAPluginInterface.h \
    SATextReadWriter.h \
    SADataHeader.h \
    SALibResourDefine.h \
    SAGlobalConfigDefine.h \
    SAVariantCaster.h \
    SAFigureGlobalConfig.h \
    SAGUIGlobalConfig.h \
    SACRC.h \
    SADataFeatureItem.h \
    SAXMLTagDefined.h \
    SAXMLReadHelper.h \
    SADataProcessVectorPointF.h \
    SAValueManagerMimeData.h \
    SAValueManagerListModel.h \
    SAHashTable.h


SOURCES += \
    SAData.cpp \
    SADataPackage.cpp \
    SAItem.cpp \
    SAValueManager.cpp \
    SAValueManagerModel.cpp \
    SARandColorMaker.cpp \
    SACsvStream.cpp \
    SALog.cpp \
    SAGlobalConfig.cpp \
    SATextReadWriter.cpp \
    SADataHeader.cpp \
    SAVariantCaster.cpp \
    SAFigureGlobalConfig.cpp \
    SAGUIGlobalConfig.cpp \
    SACRC.cpp \
    SADataFeatureItem.cpp \
    SAXMLReadHelper.cpp \
    SADataProcessVectorPointF.cpp \
    SAValueManagerMimeData.cpp \
    SAValueManagerListModel.cpp


include($$PWD/Private/Private.pri)
include($$PWD/SABaseValueType/SABaseValueType.pri)
include($$PWD/LocalServer/LocalServer.pri)

include($$PWD/../czy/czy.pri)
include($$PWD/../3rdParty/qwt/qwt_set.pri)

DEFINES += SALIB_MAKE #定义此宏将构建库
#RESOURCES += \
#    ico.qrc

RESOURCES += \
    icon.qrc
