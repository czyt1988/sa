#ifndef SALIBGLOBAL_H
#define SALIBGLOBAL_H
#include <qglobal.h>
#define SALIB_VERSION 1
#define SALIB_VERSION_STR "0.1"
#include "SAGlobals.h"


#if defined(SALIB_MAKE)     // create a DLL library
#define SALIB_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SALIB_EXPORT Q_DECL_IMPORT
#endif



#ifndef SALIB_EXPORT
#define SALIB_EXPORT
#endif

#endif // SALIBGLOBAL

