#ifndef SAHISTOGRAMSERIES_H
#define SAHISTOGRAMSERIES_H
#include "SACommonUIGlobal.h"
#include "SASeriesAndDataPtrMapper.h"
#include "qwt_plot_histogram.h"
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SAHistogramSeries : public QwtPlotHistogram,public SASeriesAndDataPtrMapper
{
public:
    explicit SAHistogramSeries(const QString &title = QString::null);
    explicit SAHistogramSeries( const QwtText &title );
    explicit SAHistogramSeries(SAAbstractDatas* intData,const QString &title = QString::null);
    using QwtPlotHistogram::setSamples;
    bool setSamples(SAAbstractDatas* intData);
};

#endif // SAHISTOGRAMSERIES_H
