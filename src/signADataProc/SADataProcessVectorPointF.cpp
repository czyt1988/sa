#include "SADataProcessVectorPointF.h"
#include "czyMath.h"
#include <algorithm>
SADataProcessVectorPointF::SADataProcessVectorPointF(QObject *parent):QObject(parent)
  ,m_sortCount(20)
{

}
///
/// \brief 设置点并开始计算
/// \param points 点集
/// \param widget 标记1
/// \param item 标记2
///
void SADataProcessVectorPointF::setPoints(const QVector<QPointF> &points, quintptr widget, quintptr item)
{
    emit result(analysisData(points),widget,item);
}

void SADataProcessVectorPointF::getVectorPointY(const QVector<QPointF> &points, QVector<double> &ys)
{
    ys.resize(points.size());
    std::transform(points.begin(),points.end(),ys.begin()
                   ,[](const QPointF& f)->double{
        return f.y();
    });
}

SADataFeatureItem* SADataProcessVectorPointF::analysisData(const QVector<QPointF>& orgPoints)
{
    SADataFeatureItem* item = new SADataFeatureItem();
    QVector<double> y;
    getVectorPointY(orgPoints,y);
    if(orgPoints.size()<=0 || y.size()<=0)
    {
        return nullptr;
    }
    item->setItemType(SADataFeatureItem::DescribeItem);
    QVector<QPointF> datas = orgPoints;
    double sum;
    double mean;
    double var;
    double stdVar;
    double skewness;
    double kurtosis;
    size_t n = datas.size();
    czy::Math::get_statistics(y.begin(),y.end(),sum,mean,var
        ,stdVar,skewness,kurtosis);
    std::sort(datas.begin(),datas.end()
              ,[](const QPointF& a,const QPointF& b)->bool{
        return (a.y() < b.y());
    });
    double min = datas.begin()->y();//最小
    double max = (datas.end()-1)->y();//最大
    double mid = n>1 ? (datas.begin() + int(n/2))->y() : min;//中位数
    double peak2peak = max - min;
    QPointF minPoint = *datas.begin();
    QPointF maxPoint = *(datas.end()-1);
    QPointF midPoint = n>1 ? *(datas.begin() + int(n/2)) : minPoint;//中位数

    item->appendItem(tr("point num"),orgPoints.size());
    item->appendItem(tr("y min"),min);
    item->appendItem(tr("y max"),max);
    item->appendItem(tr("y mid"),mid);
    item->appendItem(tr("y peak2peak"),peak2peak);
    item->appendItem(tr("y sum"),sum);
    item->appendItem(tr("y mean"),mean);
    item->appendItem(tr("y var"),var);
    item->appendItem(tr("y stdVar"),stdVar);
    item->appendItem(tr("y skewness"),skewness);
    item->appendItem(tr("y kurtosis"),kurtosis);
    item->appendItem(tr("min Point"),minPoint);
    item->appendItem(tr("max Point"),maxPoint);
    item->appendItem(tr("mid Point"),midPoint);
    QVector<QPointF> tmps;
    int sortCount = m_sortCount;
    if(sortCount > datas.size())
    {
        sortCount = datas.size();
    }
    tmps.reserve(sortCount);
    std::copy(datas.begin(),datas.begin()+sortCount,std::back_inserter(tmps));
    item->appendItem(tr("ascending order"),tmps);//升序
    tmps.clear();
    tmps.reserve(sortCount);
    std::copy(datas.rbegin(),datas.rbegin()+sortCount,std::back_inserter(tmps));
    item->appendItem(tr("descending order"),tmps);//降序
    return item;
}

int SADataProcessVectorPointF::getSortCount() const
{
    return m_sortCount;
}

void SADataProcessVectorPointF::setSortCount(int sortCount)
{
    m_sortCount = sortCount;
}
