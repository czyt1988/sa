#ifndef SASCIENCEGLOBAL_H
#define SASCIENCEGLOBAL_H

#include <QtCore/qglobal.h>

#ifndef INPUT
#define INPUT
#endif

#ifndef OUTPUT
#define OUTPUT
#endif

#ifndef IN_OUTPUT
#define IN_OUTPUT
#endif

#if defined(SASCIENCE_MAKELIB)
#  define SASCIENCE_API Q_DECL_EXPORT
#else
#  define SASCIENCE_API Q_DECL_IMPORT
#endif

#endif // SASCIENCEGLOBAL_H
