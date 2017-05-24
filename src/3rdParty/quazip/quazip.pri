INCLUDEPATH += $$PWD\include
DEPENDPATH += $$PWD


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lquazip
