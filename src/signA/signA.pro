#-------------------------------------------------
#
# Project created by QtCreator 2013-07-10T22:16:43
#
#-------------------------------------------------

message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Documentation: $$[QT_INSTALL_DOCS])
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Plugins: $$[QT_INSTALL_PLUGINS])
message(Data files: $$[QT_INSTALL_DATA])
message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
message(Settings: $$[QT_INSTALL_CONFIGURATION])
message(Examples: $$[QT_INSTALL_EXAMPLES])

DESTDIR = ../../bin
QT += core gui
QT += sql
QT += xml
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


OTHER_FILES += readme.md

DESTDIR = ../bin
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
    SAMimeData.cpp \
    SAThemeManager.cpp \
    SATabValueViewerWidget.cpp \
    SADrawDelegate.cpp \
    SAMainWindowDelegate.cpp \
    SADataImportDelegate.cpp\
    SAInformationStatusWidget.cpp \
    CurveSelectDialog.cpp \
    SAPluginManager.cpp \
    SAUI.cpp

HEADERS  += mainwindow.h \
    DebugInfo.h \
    SortIndex.h \
    def_str.h \
    progressStateWidget.h \
    AboutDialog.h \
    SAMimeData.h \
    SAThemeManager.h \
    SATabValueViewerWidget.h \
    SADrawDelegate.h \
    SAMainWindowDelegate.h \
    SADataImportDelegate.h\
    SAInformationStatusWidget.h \
    QtPropertyIdStorage.h \
    CurveSelectDialog.h \
    SAPluginManager.h \
    SAUI.h



FORMS    += mainwindow.ui \
    progressStateWidget.ui \
    AboutDialog.ui \
    SAInformationStatusWidget.ui

#INCLUDEPATH += tmp/moc/release_shared


#win32 {
    #CONFIG(debug, debug|release) {
        #DEFINES += _DEBUG
        # vld 2.2 downloaded from http://vld.codeplex.com/
        #VLD_PATH = $$quote(d:/Program Files (x86)/Visual Leak Detector/)
        #INCLUDEPATH += $VLD_PATH/include
        #LIBS += -L$VLD_PATH/lib/Win32 -lvld
    #}
#}


RESOURCES += \
    signA_Res.qrc
RC_FILE += logo.rc


#QMAKE_CFLAGS -= /GS
#QMAKE_CXXFLAGS_RELEASE  -= /GS
#QMAKE_CXXFLAGS_RELEASE  -= -GS

#3thparty
#{
#propertybrowser
include($$PWD/../3rdParty/qtpropertybrowser/src/qtpropertybrowser.pri)#propertybrowser support
#QWT
include($$PWD/../3rdParty/qwt/qwt_set.pri)
#}

#sa api support
#{
#sa api support
include($$PWD/../signALib/signALib.pri)
#sa chart support
include($$PWD/../signAChart/signAChart.pri)
#sa signACommonUI support
include($$PWD/../signACommonUI/signACommonUI.pri)
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
       # include($$PWD/nilib/NI_tdm.pri)# support the labview data : tdm/tdms
    }

    #include($$PWD/refprop4Qt/refprop4Qt.pri)# RefProp API
}

include($$PWD/mainWindows/mainWindows.pri)# the other MainWindows

include($$PWD/widget/widget.pri)# the other widget
include($$PWD/dialog/dialog.pri)# RefProp API
#qtpropertybrowser set
#include($$PWD/qtpropertybrowser/src/qtpropertybrowser.pri)#propertybrowser support
#include($$PWD/../3rdParty/qtpropertybrowser/src/qtpropertybrowser.pri)#propertybrowser support

#include($$PWD/quazip/quazip.pri)# RefProp API
# note !! the qtpropertybrowser.pri must be put at the end of this file !
#include($$PWD/gslWin32/gsl.pri)#the gsl lib support


