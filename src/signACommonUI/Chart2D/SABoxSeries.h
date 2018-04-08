#ifndef SABOXSERIES_H
#define SABOXSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "qwt_plot_tradingcurve.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SABoxSeries : public QwtPlotTradingCurve, public SASeriesAndDataPtrMapper
{
public:
    explicit SABoxSeries(const QString &title= QString::null);
    explicit SABoxSeries( const QwtText &title );
    explicit SABoxSeries(SAAbstractDatas* dataPoints,const QString &title= QString::null);
    //加载数据
    using QwtPlotTradingCurve::setSamples;
    //using QwtPlotTradingCurve::QwtSeriesStore<QwtOHLCSample>::dataSize;
    bool setSamples(SAAbstractDatas* dataPoints);

};

#endif // SABOXSERIES_H
