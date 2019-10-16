#ifndef SARPCGLOBAL_H
#define SARPCGLOBAL_H
#include <qglobal.h>
#define SARPC_VERSION 1
#define SARPC_VERSION_STR "0.1"

#include "../global/SAGlobals.h"

#if defined(SARPC_MAKE)     // create a DLL library
#define SARPC_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SARPC_EXPORT Q_DECL_IMPORT
#endif



#ifndef SARPC_EXPORT
#define SARPC_EXPORT
#endif

#endif // SARPCGLOBAL_H

