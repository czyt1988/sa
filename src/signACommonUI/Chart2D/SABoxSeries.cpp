#include "SABoxSeries.h"
#include "SAVectorOHLCDatas.h"


SABoxSeries::SABoxSeries(const QString &title):QwtPlotTradingCurve(title)
{

}

SABoxSeries::SABoxSeries(SAAbstractDatas *dataPoints,const QString &title)
    :QwtPlotTradingCurve(title)
{
    setSamples(dataPoints);
}

bool SABoxSeries::setSamples(SAAbstractDatas *dataPoints)
{
    SAVectorOHLCDatas* p = dynamic_cast<SAVectorOHLCDatas*>(dataPoints);
    if(nullptr == p)
    {
        return false;
    }
    clearDataPtrLink();
    insertData(dataPoints);
    QwtPlotTradingCurve::setSamples(p->getValueDatas());
    return true;
}

int SABoxSeries::rtti() const
{
    return SA::RTTI_SABoxSeries;
}
