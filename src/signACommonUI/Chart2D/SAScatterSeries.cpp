#include "SAScatterSeries.h"



SAScatterSeries::SAScatterSeries(const QString &title):SAXYSeries(title)
{
    setStyle( QwtPlotCurve::Dots );
}

SAScatterSeries::SAScatterSeries(const QwtText &title):SAXYSeries(title)
{
    setStyle( QwtPlotCurve::Dots );
}

SAScatterSeries::SAScatterSeries(const QString &title, SAAbstractDatas *dataPoints)
    :SAXYSeries(title)
{
    setStyle( QwtPlotCurve::Dots );
    setSamples(dataPoints);
}



