#ifndef SARIBBONGLOBAL_H
#define SARIBBONGLOBAL_H
#include <qglobal.h>
#define SARIBBON_VERSION 1
#define SARIBBON_VERSION_STR "0.1"


#ifndef SA_NO_EXPORT
    #if defined(SA_RIBBON_MAKE)     // create a DLL library
    #define SA_RIBBON_EXPORT Q_DECL_EXPORT
    #else                        // use a DLL library
    #define SA_RIBBON_EXPORT Q_DECL_IMPORT
    #endif
#endif


#ifndef SA_RIBBON_EXPORT
#define SA_RIBBON_EXPORT
#endif



#endif // SARIBBONGLOBAL_H
