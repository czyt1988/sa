INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/EasyREFPROP.h\
           $$PWD/UnitConversion.h \
            $$PWD/RefpropMaterialData.h \
            $$PWD/UnitConversion_Flow.h \
    $$PWD/materialProp.h \
    $$PWD/ThermodynamicsCalc.h

SOURCES += \
    $$PWD/materialProp.cpp \
    $$PWD/ThermodynamicsCalc.cpp

