INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += \
    $$PWD/Dialog_AddChart.cpp \
    $$PWD/Dialog_SelDefSeries.cpp \
    $$PWD/PickCurveDataModeSetDialog.cpp \
    $$PWD/SAProjectInfomationSetDialog.cpp \
    $$PWD/SAAddLineChartSetDialog.cpp \
    $$PWD/SAAddCurveTypeDialog.cpp
HEADERS  +=\
    $$PWD/Dialog_AddChart.h \
    $$PWD/Dialog_SelDefSeries.h \
    $$PWD/PickCurveDataModeSetDialog.h \
    $$PWD/SAProjectInfomationSetDialog.h \
    $$PWD/SAAddLineChartSetDialog.h \
    $$PWD/SAAddCurveTypeDialog.h
FORMS    +=\
    $$PWD/Dialog_AddChart.ui \
    $$PWD/Dialog_SelDefSeries.ui \
    $$PWD/PickCurveDataModeSetDialog.ui \
    $$PWD/SAProjectInfomationSetDialog.ui \
    $$PWD/SAAddLineChartSetDialog.ui \
    $$PWD/SAAddCurveTypeDialog.ui

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
