#ifndef SAINTERVALSERIES_H
#define SAINTERVALSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "qwt_plot_intervalcurve.h"
class SAAbstractDatas;

class SA_COMMON_UI_EXPORT SAIntervalSeries : public QwtPlotIntervalCurve
{
public:
    explicit SAIntervalSeries( const QString &title = QString() );
    explicit SAIntervalSeries( const QwtText &title );
    explicit SAIntervalSeries(const QString &title,SAAbstractDatas* intervalVal);
    //跨域重载
    using QwtPlotIntervalCurve::setSamples;
    //加载数据
    bool setSamples(const SAAbstractDatas* intervalVal);
    bool setSamples(const SAAbstractDatas* x,const SAAbstractDatas* yMin,const SAAbstractDatas* yMax);

};

#endif // SAINTERVALSERIES_H
