#ifndef SASCATTERSERIES_H
#define SASCATTERSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "SAXYSeries.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SAScatterSeries : public SAXYSeries,SASeriesAndDataPtrMapper
{
public:
    SAScatterSeries();
    SAScatterSeries(const QString &title);
    SAScatterSeries(const QString &title,SAAbstractDatas* dataPoints);
    //跨域重载
    using QwtPlotCurve::setSamples;
    using SAXYSeries::setSamples;
};

#endif // SASCATTERSERIES_H
