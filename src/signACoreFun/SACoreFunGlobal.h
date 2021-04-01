#ifndef SACOREFUNGLOBAL
#define SACOREFUNGLOBAL

#include <qglobal.h>
#include <cmath>
#include <cfloat>
#include <algorithm>

#if defined(SA_CORE_FUN_MAKE)     // create a DLL library
#define SA_CORE_FUN__EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SA_CORE_FUN__EXPORT Q_DECL_IMPORT
#endif

#ifndef SA_NAN
#define SA_NAN std::nan()
#endif
#ifndef SA_NANF
#define SA_NANF std::nanf()
#endif

#ifndef SA_CORE_FUN__EXPORT
#define SA_CORE_FUN__EXPORT
#endif

#ifndef SA_FUNCTION_DEFINE
#define SA_FUNCTION_DEFINE(funName)\
    bool SA_CORE_FUN__EXPORT funName(QList<SAAbstractDatas *> &input,QList<SAAbstractDatas *> &output,QString* info)
#endif


#endif // SACOREFUNGLOBAL

