#ifndef SASCATTERSERIES_H
#define SASCATTERSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "SAXYSeries.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SAScatterSeries : public SAXYSeries
{
public:
    SAScatterSeries();
    SAScatterSeries(const QString &title);
    SAScatterSeries(const QString &title,SAAbstractDatas* dataPoints);
    //跨域重载
    using QwtPlotCurve::setSamples;
    using SAXYSeries::setSamples;
    virtual int rtti() const Q_DECL_OVERRIDE;
};

#endif // SASCATTERSERIES_H
