INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += \
    $$PWD/Dialog_AddChart.cpp \
    $$PWD/Dialog_SelDefSeries.cpp \
    $$PWD/Dialog_ChooseCurve.cpp \
    $$PWD/dialog_renameproject.cpp \
    $$PWD/TendencyChartDataModeSel.cpp \
    $$PWD/PickCurveDataModeSetDialog.cpp \
    $$PWD/SAProjectInfomationSetDialog.cpp
HEADERS  +=\
    $$PWD/Dialog_AddChart.h \
    $$PWD/Dialog_SelDefSeries.h \
    $$PWD/Dialog_ChooseCurve.h \
    $$PWD/dialog_renameproject.h\
    $$PWD/TendencyChartDataModeSel.h \
    $$PWD/PickCurveDataModeSetDialog.h \
    $$PWD/SAProjectInfomationSetDialog.h
FORMS    +=\
    $$PWD/dialog_renameproject.ui \
    $$PWD/Dialog_AddChart.ui \
    $$PWD/Dialog_SelDefSeries.ui \
    $$PWD/Dialog_ChooseCurve.ui \
    $$PWD/TendencyChartDataModeSel.ui \
    $$PWD/PickCurveDataModeSetDialog.ui \
    $$PWD/SAProjectInfomationSetDialog.ui

win32{
SOURCES +=
HEADERS  +=
FORMS    +=
contains(DEFINES,SA_TDMS){
    SOURCES +=
    HEADERS  +=
    FORMS    +=
    }
}

RESOURCES +=
