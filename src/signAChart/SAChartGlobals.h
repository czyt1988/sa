#ifndef SACHARTGLOBALS_H
#define SACHARTGLOBALS_H

#include <qglobal.h>
#define SACHARTLIB_VERSION 1
#define SACHARTLIB_VERSION_STR "0.1"



#if defined(SA_CHART_MAKE)     // create a DLL library
#define SA_CHART_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SA_CHART_EXPORT Q_DECL_IMPORT
#endif



#ifndef SA_CHART_EXPORT
#define SA_CHART_EXPORT
#endif

#ifndef SARttiMarker_LowerBoundary
#define SARttiMarker_LowerBoundary (QwtPlotItem::Rtti_PlotUserItem+1)
#endif
#ifndef SARttiMarker_UpperBoundary
#define SARttiMarker_UpperBoundary (QwtPlotItem::Rtti_PlotUserItem+100)
#endif

#endif // SACHARTGLOBALS

