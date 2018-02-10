#include "SAScatterSeries.h"

SAScatterSeries::SAScatterSeries():SAXYSeries()
{
    setStyle( QwtPlotCurve::Dots );
}

SAScatterSeries::SAScatterSeries(const QString &title):SAXYSeries(title)
{
    setStyle( QwtPlotCurve::Dots );
}

SAScatterSeries::SAScatterSeries(const QString &title, SAAbstractDatas *dataPoints)
    :SAXYSeries(title)
{
    setStyle( QwtPlotCurve::Dots );
}



