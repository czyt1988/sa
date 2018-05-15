#include "SAIntervalSeries.h"
#include "SAVectorInterval.h"
#include "SADataConver.h"
SAIntervalSeries::SAIntervalSeries(const QString &title) : QwtPlotIntervalCurve(title)
{

}

SAIntervalSeries::SAIntervalSeries(const QwtText &title) : QwtPlotIntervalCurve(title)
{

}

SAIntervalSeries::SAIntervalSeries(const QString &title, SAAbstractDatas *intervalVal) : QwtPlotIntervalCurve(title)
{
    setSamples(intervalVal);
}

bool SAIntervalSeries::setSamples(const SAAbstractDatas *intervalVal)
{
    if(SA::VectorInterval != intervalVal->getType())
    {
        return false;
    }
    const SAVectorInterval* ser = static_cast<const SAVectorInterval*>(intervalVal);
    setSamples(ser->getValueDatas());
    return true;
}

bool SAIntervalSeries::setSamples(const SAAbstractDatas *x,const SAAbstractDatas *yMin,const SAAbstractDatas *yMax)
{
    QVector<QwtIntervalSample> datas;
    {
        QVector<double> vx,vymin,vymax;
        if(!SADataConver::converToDoubleVector(x,vx))
        {
            return false;
        }
        if(!SADataConver::converToDoubleVector(yMin,vymin))
        {
            return false;
        }
        if(!SADataConver::converToDoubleVector(yMax,vymax))
        {
            return false;
        }
        int minSize = qMin(qMin(vx.size(),vymin.size()),vymax.size());
        datas.reserve(minSize);
        for(int i=0;i<minSize;++i)
        {
            datas.push_back(QwtIntervalSample(vx[i],vymin[i],vymax[i]));
        }
    }
    if(datas.size() > 0)
    {
        setSamples(datas);
        return true;
    }
    return false;
}

