#ifndef SAUTILGLOBAL_H
#define SAUTILGLOBAL_H

#ifndef INPUT
#define INPUT
#endif

#ifndef OUTPUT
#define OUTPUT
#endif

#ifndef IN_OUTPUT
#define IN_OUTPUT
#endif

#include <stdlib.h>
#include <stdio.h>



/// \def 打印字节十六进制
#ifndef printByteArray
#define printByteArray(dataPtr,dataLen) \
    do{\
        const char* __c__char__ptr__ = (const char*)(dataPtr);\
        fprintf(stderr,"[");\
        for(int M__i=0;M__i<(dataLen);++M__i)\
        {\
            if(M__i % 10 == 0)\
            {\
                fprintf(stderr,"\n");\
                fprintf(stderr,">> [%d]-[%d]: ",M__i,M__i+9);\
            }\
            fprintf(stderr,"%02X,",(int)__c__char__ptr__[M__i]);\
        }\
        fprintf(stderr,"\n]");\
    }while(0)
#endif

#include "../global/SAGlobals.h"

#if defined(SAUTIL_MAKE)     // create a DLL library
#define SAUTIL_API Q_DECL_EXPORT
#else                        // use a DLL library
#define SAUTIL_API Q_DECL_IMPORT
#endif

#endif // SAUTILGLOBAL_H
