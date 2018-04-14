#include "SAScatterSeries.h"



SAScatterSeries::SAScatterSeries(const QString &title):SAXYSeries(title)
{

}

SAScatterSeries::SAScatterSeries(const QwtText &title):SAXYSeries(title)
{

}

SAScatterSeries::SAScatterSeries(const QString &title, SAAbstractDatas *dataPoints)
    :SAXYSeries(title)
{
    setStyle( QwtPlotCurve::Dots );
}



