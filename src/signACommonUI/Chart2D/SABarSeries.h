#ifndef SABARSERIES_H
#define SABARSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "qwt_plot_barchart.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SABarSeries : public QwtPlotBarChart,public SASeriesAndDataPtrMapper
{
public:
    SABarSeries(const QString &title = QString::null);
    SABarSeries(SAAbstractDatas* intData,const QString &title = QString::null);
    using QwtPlotBarChart::setSamples;
    bool setSamples(SAAbstractDatas* intData);
};

#endif // SABARSERIES_H
