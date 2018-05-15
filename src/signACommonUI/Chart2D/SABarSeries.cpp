#include "SABarSeries.h"
#include "SAVectorInterval.h"
#include "SADataConver.h"
SABarSeries::SABarSeries(const QString &title):QwtPlotBarChart(title)
{
    setLayoutPolicy(AutoAdjustSamples);
    setLegendMode( QwtPlotBarChart::LegendBarTitles );
    setLegendIconSize( QSize( 10, 14 ) );
    setLayoutPolicy( AutoAdjustSamples );
    setLayoutHint( 4.0 ); // minimum width for a single bar
    setSpacing( 10 ); // spacing between bars
}

SABarSeries::SABarSeries(const QwtText &title):QwtPlotBarChart(title)
{
    setLayoutPolicy(AutoAdjustSamples);
    setLegendMode( QwtPlotBarChart::LegendBarTitles );
    setLegendIconSize( QSize( 10, 14 ) );
    setLayoutPolicy( AutoAdjustSamples );
    setLayoutHint( 4.0 ); // minimum width for a single bar
    setSpacing( 10 ); // spacing between bars
}

SABarSeries::SABarSeries(SAAbstractDatas *intData, const QString &title)
    :QwtPlotBarChart(title)
{
    setLayoutPolicy(AutoAdjustSamples);
    setLegendMode( QwtPlotBarChart::LegendBarTitles );
    setLegendIconSize( QSize( 10, 14 ) );
    setLayoutPolicy( AutoAdjustSamples );
    setLayoutHint( 4.0 ); // minimum width for a single bar
    setSpacing( 10 ); // spacing between bars
    setSamples(intData);
}

bool SABarSeries::setSamples(SAAbstractDatas *intData)
{
    if(SA::Dim1 == intData->getDim())
    {
        QVector<double> serPoints;
        if(!SADataConver::converToDoubleVector(intData,serPoints))
        {
            return false;
        }
        if(serPoints.size() <= 0)
        {
            return false;
        }
        clearDataPtrLink();
        insertData(intData);
        QwtPlotBarChart::setSamples(serPoints);
        return true;
    }
    else if(SA::Dim2 == intData->getDim())
    {
        QVector<QPointF> serPoints;
        if(!SADataConver::converToPointFVector(intData,serPoints))
        {
            return false;
        }
        if(serPoints.size() <= 0)
        {
            return false;
        }
        clearDataPtrLink();
        insertData(intData);
        QwtPlotBarChart::setSamples(serPoints);
        return true;
    }
    return false;
}

