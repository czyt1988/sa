#ifndef SAPROCESSGLOBAL
#define SAPROCESSGLOBAL

#include <qglobal.h>
#include "../global/SAGlobals.h"

#if defined(SA_PROCESS_MAKE)     // create a DLL library
#define SA_PROCESS_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SA_PROCESS_EXPORT Q_DECL_IMPORT
#endif



#ifndef SA_PROCESS_EXPORT
#define SA_PROCESS_EXPORT
#endif



#endif // SAPROCESSGLOBAL
