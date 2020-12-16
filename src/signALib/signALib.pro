#-------------------------------------------------
#
# Project created by czy 2016-10-14
# sa api lib
#
#-------------------------------------------------

message("------------signALib SA核心库-------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" #忽略warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
QT += core gui xml
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = signALib
TEMPLATE = lib

include(../sa_common.pri)
DESTDIR = $$SA_LIBDIR

CONFIG += c++11

HEADERS += \
    $$PWD/../global/SAGlobals.h \
    SAData.h \
    SADataPackage.h \
    SAItem.h \
    SALibGlobal.h \
    SALineGradientColorList.h \
    SAOrderMap.h \
    SAPoint.h \
    SATable.h \
    SAThreadPool.h \
    SAValueManager.h \
    SAValueManagerModel.h \
    SARandColorMaker.h \
    SACsvStream.h \
    SALog.h \ 
    SAPluginInterface.h \
    SATextReadWriter.h \
    SADataHeader.h \
    SALibResourDefine.h \
    SAValueManagerMimeData.h \
    SAValueManagerListModel.h \
    SAHashTable.h \
    SATree.h \
    SATreeModel.h


SOURCES += \
    SAData.cpp \
    SADataPackage.cpp \
    SAItem.cpp \
    SALineGradientColorList.cpp \
    SAPoint.cpp \
    SAThreadPool.cpp \
    SAValueManager.cpp \
    SAValueManagerModel.cpp \
    SARandColorMaker.cpp \
    SACsvStream.cpp \
    SALog.cpp \
    SATextReadWriter.cpp \
    SADataHeader.cpp \
    SAValueManagerMimeData.cpp \
    SAValueManagerListModel.cpp \
    SATree.cpp \
    SATreeModel.cpp

#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)
include($$PWD/../signAUtil/signAUtil.pri)
include($$PWD/Private/Private.pri)
include($$PWD/SABaseValueType/SABaseValueType.pri)
include($$PWD/../3rdParty/qwt/qwt_set.pri)
DEFINES += SALIB_MAKE #定义此宏将构建库
#RESOURCES += \
#    ico.qrc

RESOURCES += \
    icon.qrc
