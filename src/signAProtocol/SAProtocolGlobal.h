#ifndef SAPROTOCOLGLOBAL
#define SAPROTOCOLGLOBAL

#include <qglobal.h>
#include "../global/SAGlobals.h"

#if defined(SA_PROTOCOL_MAKE)     // create a DLL library
#define SA_PROTOCOL_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SA_PROTOCOL_EXPORT Q_DECL_IMPORT
#endif



#ifndef SA_PROTOCOL_EXPORT
#define SA_PROTOCOL_EXPORT
#endif



#endif // SAPROCESSGLOBAL
