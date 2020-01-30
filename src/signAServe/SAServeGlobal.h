#ifndef SARPCGLOBAL_H
#define SARPCGLOBAL_H
#include <qglobal.h>
#define SASERVE_VERSION 1
#define SASERVE_VERSION_STR "0.1"

#include "../global/SAGlobals.h"

#if defined(SASERVE_MAKE)     // create a DLL library
#define SASERVE_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SASERVE_EXPORT Q_DECL_IMPORT
#endif



#ifndef SASERVE_EXPORT
#define SASERVE_EXPORT
#endif

#endif // SARPCGLOBAL_H

