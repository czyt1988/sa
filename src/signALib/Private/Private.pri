#在这里的类都是不导出的类，由于不导出，不允许在导出类的头文件进行include
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/SAConfigXMLReadWriter.h

SOURCES += \
    $$PWD/SAConfigXMLReadWriter.cpp
