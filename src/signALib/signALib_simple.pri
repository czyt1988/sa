INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/SABaseValueType
DEPENDPATH += $$PWD/SABaseValueType

HEADERS += \
    $$PWD/SADataPackage.h \
    $$PWD/SAItem.h \
    $$PWD/SAGlobals.h \
    $$PWD/SALibGlobal.h \
    $$PWD/SADataTypeInfo.h\
    $$PWD/SAValueManager.h \
    $$PWD/SAValueManagerModel.h\
    $$PWD/SAUIReflection.h\
    $$PWD/SARandColorMaker.h\
    $$PWD/SACsvWriter.h\
    $$PWD/SALog.h\
    $$PWD/SAGlobalConfig.h\
\
    $$PWD/SABaseValueType/SAAbstractDatas.h \
    $$PWD/SABaseValueType/SADataReference.h \
    $$PWD/SABaseValueType/SAVectorDatas.h \
    $$PWD/SABaseValueType/SAVectorInt.h \
    $$PWD/SABaseValueType/SAVectorDouble.h \
    $$PWD/SABaseValueType/SAVectorVariant.h \
    $$PWD/SABaseValueType/SAVectorPointF.h \
    $$PWD/SABaseValueType/SATableData.h \
    $$PWD/SABaseValueType/SATableVariant.h \
    $$PWD/SABaseValueType/SATableDouble.h


LIBS += -L$$PWD/../bin/libs/ -lsignALib
