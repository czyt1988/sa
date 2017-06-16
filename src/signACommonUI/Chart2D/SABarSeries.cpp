#include "SABarSeries.h"
#include "SAVectorInterval.h"
SABarSeries::SABarSeries(const QString &title):QwtPlotHistogram(title)
{

}

SABarSeries::SABarSeries(SAAbstractDatas *intData, const QString &title):QwtPlotHistogram(title)
{
    setSamples(intData);
}

bool SABarSeries::setSamples(SAAbstractDatas *intData)
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
