#ifndef SACOMMONUIGLOBAL_H
#define SACOMMONUIGLOBAL_H
#include <qglobal.h>
#define SALIB_VERSION 1
#define SALIB_VERSION_STR "0.1"



#if defined(SA_COMMON_UI_MAKE)     // create a DLL library
#define SA_COMMON_UI_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SA_COMMON_UI_EXPORT Q_DECL_IMPORT
#endif



#ifndef SA_COMMON_UI_EXPORT
#define SA_COMMON_UI_EXPORT
#endif



#endif // SALIBGLOBAL

