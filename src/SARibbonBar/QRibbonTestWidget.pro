#-------------------------------------------------
#
# Project created by QtCreator 2017-09-21T14:04:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRibbonTestWidget
TEMPLATE = app


SOURCES += main.cpp\
    SARibbonBar.cpp \
    SARibbonMainWindow.cpp \
    FramelessHelper.cpp \
    SAWindowButtonGroup.cpp \
    SARibbonApplicationButton.cpp \
    SARibbonTabBar.cpp \
    SARibbonCategory.cpp \
    mainwindow.cpp \
    SARibbonContextCategory.cpp \
    SARibbonPannel.cpp \
    SARibbonToolButton.cpp \
    SARibbonMenu.cpp \
    SARibbonPannelOptionButton.cpp \
    SARibbonSeparatorWidget.cpp \
    Form.cpp \
    SARibbonCategoryLayout.cpp \
    SARibbonGallery.cpp \
    SARibbonControlButton.cpp \
    SARibbonGalleryGroup.cpp \
    SARibbonGalleryItem.cpp \
    SARibbonComboBox.cpp \
    SARibbonElementCreateDelegate.cpp \
    SARibbonElementManager.cpp \
    SARibbonDrawHelper.cpp \
    SARibbonCtrolContainer.cpp \
    SARibbonLineEdit.cpp \
    SARibbonCheckBox.cpp

HEADERS  += \
    SARibbonBar.h \
    SARibbonMainWindow.h \
    FramelessHelper.h \
    SAWindowButtonGroup.h \
    SARibbonApplicationButton.h \
    SARibbonTabBar.h \
    SARibbonCategory.h \
    mainwindow.h \
    SARibbonContextCategory.h \
    SARibbonPannel.h \
    SARibbonToolButton.h \
    SARibbonMenu.h \
    SARibbonGlobal.h \
    SARibbonPannelOptionButton.h \
    SARibbonSeparatorWidget.h \
    Form.h \
    SARibbonCategoryLayout.h \
    SARibbonGallery.h \
    SARibbonControlButton.h \
    SARibbonGalleryGroup.h \
    SARibbonGalleryItem.h \
    SARibbonComboBox.h \
    SARibbonElementCreateDelegate.h \
    SARibbonElementManager.h \
    SARibbonDrawHelper.h \
    SARibbonCtrolContainer.h \
    SARibbonLineEdit.h \
    SARibbonCheckBox.h

RESOURCES += \
    saribbonresouce.qrc \
    resource.qrc

DEFINES += SA_NO_EXPORT

FORMS += \
    Form.ui
