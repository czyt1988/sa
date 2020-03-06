#ifndef SASCIENCEGLOBAL_H
#define SASCIENCEGLOBAL_H

#include <QtCore/qglobal.h>
#include "../global/SAGlobals.h"
#include "SAScienceDefine.h"

#if defined(SASCIENCE_MAKELIB)
#  define SASCIENCE_API Q_DECL_EXPORT
#else
#  define SASCIENCE_API Q_DECL_IMPORT
#endif

#endif // SASCIENCEGLOBAL_H
