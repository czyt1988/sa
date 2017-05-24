#pragma once
#ifndef qwt3d_global_h__2009_10_11_14_19_14_begin_guarded_code
#define qwt3d_global_h__2009_10_11_14_19_14_begin_guarded_code

//#include <qglobal.h>

#define QWT3D_MAJOR_VERSION 0
#define QWT3D_MINOR_VERSION 3
#define QWT3D_PATCH_VERSION 1

//
// Create Qwt3d DLL if QWT3D_DLL is defined (Windows only)
//

#if defined(Q_OS_WIN)
  #if defined(_MSC_VER) /* MSVC Compiler */
    #pragma warning(disable: 4251) // dll interface required for stl templates
	   //pragma warning(disable: 4244) // 'conversion' conversion from 'type1' to 'type2', possible loss of data
    #pragma warning(disable: 4786) // truncating debug info after 255 characters
    #pragma warning(disable: 4660) // template-class specialization 'identifier' is already instantiated
    #if (_MSC_VER >= 1400) /* VS8 - not sure about VC7 */
      #pragma warning(disable: 4996) /* MS security enhancements */
    #endif
  #endif

  #if defined(QWT3D_NODLL)
    #undef QWT3D_MAKEDLL
    #undef QWT3D_DLL
    #undef QWT3D_TEMPLATEDLL
  #endif

  #ifdef QWT3D_DLL
    #if defined(QWT3D_MAKEDLL)     /* create a Qwt3d DLL library */
      #undef QWT3D_DLL
      #define QWT3D_EXPORT  __declspec(dllexport)
      #define QWT3D_TEMPLATEDLL
    #endif
  #endif

  #if defined(QWT3D_DLL)     /* use a Qwt3d DLL library */
    #define QWT3D_EXPORT  __declspec(dllimport)
    #define QWT3D_TEMPLATEDLL
  #endif

#else // ! Q_OS_WIN
  #undef QWT3D_MAKEDLL       /* ignore these for other platforms */
  #undef QWT3D_DLL
  #undef QWT3D_TEMPLATEDLL
#endif

#ifndef QWT3D_EXPORT
  #define QWT3D_EXPORT
#endif

#endif /* include guard */
