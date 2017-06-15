#include "SAXYSeries.h"
#include "SAAbstractDatas.h"
SAXYSeries::SAXYSeries():QwtPlotCurve()
{

}

SAXYSeries::SAXYSeries(const QString &title):QwtPlotCurve(title)
{

}

SAXYSeries::SAXYSeries(const QString &title, SAAbstractDatas *dataPoints):QwtPlotCurve(title)
{
    setSamples(dataPoints);
}
///
/// \brief 加载数据
/// \param dataPoints
/// \return
///
bool SAXYSeries::setSamples(SAAbstractDatas *dataPoints)
{
    QVector<QPointF> serPoints;
    if(!SAAbstractDatas::converToPointFVector(dataPoints,serPoints))
    {
        return false;
    }
    if(serPoints.size() <= 0)
    {
        return false;
    }
    QwtPlotCurve::setSamples(serPoints);
    m_linkDatas.insert(dataPoints);
    return true;
}
///
/// \brief 加载数据
/// \param x x值
/// \param y y值
/// \return
///
bool SAXYSeries::setSamples(SAAbstractDatas *x, SAAbstractDatas *y)
{
    QVector<double> xd,yd;
    if(!SAAbstractDatas::converToDoubleVector(x,xd))
        return false;
    if(!SAAbstractDatas::converToDoubleVector(y,yd))
        return false;
    if(0 == xd.size() || 0 == yd.size())
        return false;
    QwtPlotCurve::setSamples(xd.data(),yd.data(),qMin(xd.size(),yd.size()));
}
///
/// \brief SAXYSeries::setSamples
/// \param y
/// \param xStart
/// \param xDetal
/// \return
///
bool SAXYSeries::setSamples(SAAbstractDatas *y, double xStart, double xDetal)
{
    QVector<double> yd;
    if(!SAAbstractDatas::converToDoubleVector(y,yd))
        return false;
    if(0 == yd.size())
        return false;
    QVector<double> xd;
    for(int i=0;i<yd.size();++i)
    {
        xd.append(xStart + (i*xDetal));
    }
    QwtPlotCurve::setSamples(xd,yd);
}

QSet<SAAbstractDatas *> SAXYSeries::linkDatas() const
{
    return m_linkDatas;
}

bool SAXYSeries::isContainData(SAAbstractDatas *d)
{
    return m_linkDatas.contains(d);
}

void SAXYSeries::removeLinkDataPtr(const SAAbstractDatas *ptr)
{
    m_linkDatas.remove(const_cast<SAAbstractDatas *>(ptr));
}


