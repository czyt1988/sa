#ifndef SACOMMONUIGLOBAL_H
#define SACOMMONUIGLOBAL_H
#include <qglobal.h>
#define SALIB_VERSION 1
#define SALIB_VERSION_STR "0.1"
#include "../global/SAGlobals.h"


#if defined(SA_COMMON_UI_MAKE)     // create a DLL library
#define SA_COMMON_UI_EXPORT Q_DECL_EXPORT
#else                        // use a DLL library
#define SA_COMMON_UI_EXPORT Q_DECL_IMPORT
#endif



#ifndef SA_COMMON_UI_EXPORT
#define SA_COMMON_UI_EXPORT
#endif
#include "qwt_plot_item.h"
namespace SA {
    enum RTTI_SAChart2D{
//        RTTI_SAXYSeries = QwtPlotItem::Rtti_PlotUserItem+10
//        ,RTTI_SABarSeries = QwtPlotItem::Rtti_PlotUserItem+11
//        ,RTTI_SABoxSeries = QwtPlotItem::Rtti_PlotUserItem+12
//        ,RTTI_SAHistogramSeries = QwtPlotItem::Rtti_PlotUserItem+13
//        ,RTTI_SAScatterSeries = QwtPlotItem::Rtti_PlotUserItem+14
        RTTI_SASelectRegionDataEditor = QwtPlotItem::Rtti_PlotUserItem+500
        ,RTTI_SASelectRegionEditor = QwtPlotItem::Rtti_PlotUserItem+501
    };
}

#endif // SALIBGLOBAL

