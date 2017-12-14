#ifndef SAHISTOGRAMSERIES_H
#define SAHISTOGRAMSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "qwt_plot_histogram.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SAHistogramSeries : public QwtPlotHistogram,public SASeriesAndDataPtrMapper
{
public:
    SAHistogramSeries(const QString &title = QString::null);
    SAHistogramSeries(SAAbstractDatas* intData,const QString &title = QString::null);
    using QwtPlotHistogram::setSamples;
    bool setSamples(SAAbstractDatas* intData);
    virtual int rtti() const Q_DECL_OVERRIDE;
};

#endif // SAHISTOGRAMSERIES_H
