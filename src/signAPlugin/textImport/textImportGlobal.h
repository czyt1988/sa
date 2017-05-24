#ifndef TEXTIMPORTGLOBAL_H
#define TEXTIMPORTGLOBAL_H
#if defined(SALIB_MAKE)     // create a DLL library
#define SALIB_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SALIB_EXPORT Q_DECL_IMPORT
#endif



#ifndef SALIB_EXPORT
#define SALIB_EXPORT
#endif

#endif // TEXTIMPORTGLOBAL_H
