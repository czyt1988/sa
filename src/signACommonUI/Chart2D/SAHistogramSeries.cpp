#include "SAHistogramSeries.h"
#include "SAVectorInterval.h"

SAHistogramSeries::SAHistogramSeries(const QString &title):QwtPlotHistogram(title)
{

}

SAHistogramSeries::SAHistogramSeries(SAAbstractDatas *intData, const QString &title)
    :QwtPlotHistogram(title)
{
    setSamples(intData);
}

bool SAHistogramSeries::setSamples(SAAbstractDatas *intData)
{
    SAVectorInterval* p = dynamic_cast<SAVectorInterval*>(intData);
    if(nullptr == p)
    {
        return false;
    }
    clearDataPtrLink();
    insertData(intData);
    QwtPlotHistogram::setSamples(p->getValueDatas());
    return true;
}

