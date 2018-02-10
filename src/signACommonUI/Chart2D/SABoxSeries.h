#ifndef SABOXSERIES_H
#define SABOXSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "qwt_plot_tradingcurve.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SABoxSeries : public QwtPlotTradingCurve, public SASeriesAndDataPtrMapper
{
public:
    SABoxSeries(const QString &title= QString::null);
    SABoxSeries(SAAbstractDatas* dataPoints,const QString &title= QString::null);
    //加载数据
    using QwtPlotTradingCurve::setSamples;
    //using QwtPlotTradingCurve::QwtSeriesStore<QwtOHLCSample>::dataSize;
    bool setSamples(SAAbstractDatas* dataPoints);

};

#endif // SABOXSERIES_H
