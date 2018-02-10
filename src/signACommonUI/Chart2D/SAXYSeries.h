#ifndef SAXYSERIES_H
#define SAXYSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "qwt_plot_curve.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SAXYSeries : public QwtPlotCurve,public SASeriesAndDataPtrMapper
{
public:
    SAXYSeries();
    SAXYSeries(const QString &title);
    SAXYSeries(const QString &title,SAAbstractDatas* dataPoints);
    //跨域重载
    using QwtPlotCurve::setSamples;
    //加载数据
    bool setSamples(SAAbstractDatas* dataPoints);
    bool setSamples(SAAbstractDatas* x,SAAbstractDatas* y);
    bool setSamples(SAAbstractDatas* y,double xStart,double xDetal);
};



#endif // SAXYSERIES_H
