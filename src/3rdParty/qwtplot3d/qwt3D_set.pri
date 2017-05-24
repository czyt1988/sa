INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QWT3D_ROOT = $$PWD
QT += opengl
INCLUDEPATH += $${QWT3D_ROOT}/src
DEPENDPATH += $${QWT3D_ROOT}/src

win32{
  !build_pass {
    win32-msvc | win32-msvc2002 {
      error(Unsupported Visual Studio version ( < 2003 ))
    }
  }

  win32-msvc2008 | win32-msvc2010 | win32-msvc2012 | win32-msvc2013 {
    QMAKE_CXXFLAGS += -MP
    QMAKE_CXXFLAGS += $$QMAKE_CFLAGS_STL
  }
#LIBS += -lopengl32 -lglu32 -lgdi32
CONFIG(debug, debug|release):LIBS += -L$${QWT3D_ROOT}/lib/d -lqwtplot3d -lopengl32 -lglu32 -lgdi32
CONFIG(release, debug|release):LIBS += -L$${QWT3D_ROOT}/lib/r -lqwtplot3d -lopengl32 -lglu32 -lgdi32

}
unix:!macx {
    CONFIG(debug, debug|release):LIBS += -L$${QWT3D_ROOT}/lib/d -lqwtplot3d -lGLU
    CONFIG(release, debug|release):LIBS += -L$${QWT3D_ROOT}/lib/r -lqwtplot3d -lGLU
}
macx: {
    CONFIG(debug, debug|release):LIBS += -L$${QWT3D_ROOT}/lib/d -lqwtplot3d -framework OpenGL
    CONFIG(release, debug|release):LIBS += -L$${QWT3D_ROOT}/lib/r -lqwtplot3d -framework OpenGL
}
linux-g++:QMAKE_CXXFLAGS += -fno-exceptions
#LIBS += -lopengl32 -lglu32 -lgdi32
