#-------------------------------------------------
#
# Project created by QtCreator 2013-07-10T22:16:43
#
#-------------------------------------------------
message("")
message("--------------SA---------------------------")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_debug
}else {
    DESTDIR = $$PWD/../bin_qt$$[QT_VERSION]_release
}

QT += core gui
QT += sql
QT += xml
QT += opengl
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


DEFINES -= SA_TDMS


TARGET = signA
TEMPLATE = app
CONFIG += c++11
INCLUDEPATH += $$PWD
#win32: LIBS += -luser32
#include($$PWD/3rdparty/qwtplot3d/qwt3D_set.pri)#the qwt support

SOURCES += main.cpp\
    mainwindow.cpp \
    progressStateWidget.cpp \
    AboutDialog.cpp \   
    SAThemeManager.cpp \
    SATabValueViewerWidget.cpp \
    SADrawDelegate.cpp \
    SAMainWindowDelegate.cpp \
    SAInformationStatusWidget.cpp \
    CurveSelectDialog.cpp \
    SAPluginManager.cpp \
    SAUI.cpp \
    MainWindowPrivate.cpp \
    SAValueDataTableView.cpp \
    SAChartDataTableView.cpp \
    SADataClient.cpp

HEADERS  += mainwindow.h \
    DebugInfo.h \
    progressStateWidget.h \
    AboutDialog.h \
    SAThemeManager.h \
    SATabValueViewerWidget.h \
    SADrawDelegate.h \
    SAMainWindowDelegate.h \
    SAInformationStatusWidget.h \
    QtPropertyIdStorage.h \
    CurveSelectDialog.h \
    SAPluginManager.h \
    SAUI.h \
    SAResourDefine.h \
    MainWindowPrivate.h \
    SAValueDataTableView.h \
    SAChartDataTableView.h \
    SADataClient.h



FORMS    += \
    progressStateWidget.ui \
    AboutDialog.ui \
    SAInformationStatusWidget.ui

#INCLUDEPATH += tmp/moc/release_shared
OTHER_FILES += \
    readme.md\
    $$PWD/../readme.md\
    logo.rc

RESOURCES += \
    icons.qrc \
    theme.qrc
RC_FILE += logo.rc

#include($$[QT_INSTALL_PREFIX]/../Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri)

#3thparty
#{
#QWT
include($$PWD/../3rdParty/qwt/qwt_set.pri)
#}

#sa api support
#{
#sa protocol support
include($$PWD/../signAProtocol/signAProtocol.pri)
#sa serve support
include($$PWD/../signAServe/signAServe.pri)
#sa api support
include($$PWD/../signALib/signALib.pri)
#sa multiprocessing process support
include($$PWD/../signAProcess/signAProcess.pri)
#sa chart support
include($$PWD/../signAChart/signAChart.pri)
#sa signACommonUI support
include($$PWD/../signACommonUI/signACommonUI.pri)
#sa ribbon support
include($$PWD/../SARibbonBar/SARibbon/SARibbonBar.pri)
#}


#the czy extern support
#{
include($$PWD/../czy/czy.pri)#the czy extern support
#}

include($$PWD/model/model.pri)# the seldefine model
win32{
    message("win32 set")
    #LIBS += -luser32
    contains(DEFINES,SA_TDMS){
        message("SA_TDMS is definded")
    }
}

include($$PWD/mainWindows/mainWindows.pri)# the other MainWindows
include($$PWD/widget/widget.pri)# the other widget
include($$PWD/dialog/dialog.pri)# RefProp API


