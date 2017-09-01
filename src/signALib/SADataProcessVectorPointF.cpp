#include "SADataProcessVectorPointF.h"
#include "czyMath.h"
#include <algorithm>

#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#include <QDebug>
#endif

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
void SADataProcessVectorPointF::setPoints(const QVector<QPointF> &points, QVariant args)
{
    emit result(analysisData(points),args);
}

void SADataProcessVectorPointF::getVectorPointY(const QVector<QPointF> &points, QVector<double> &ys)
{
    ys.resize(points.size());
    std::transform(points.begin(),points.end(),ys.begin()
                   ,[](const QPointF& f)->double{
        return f.y();
    });
}
///
/// \brief 获取尖峰的点 - 所谓尖峰是指三点a,b,c b>a && b>c 就说明b是尖峰
/// \param sharpPoints 尖峰值引用
/// \param Points 需要获得的数据
/// \param isUpperPeak 获取的是上峰值
///
void SADataProcessVectorPointF::getSharpPeakPoint(QVector<QPointF> &sharpPoints, const QVector<QPointF> &points, bool isUpperPeak)
{
    sharpPoints.clear();
    sharpPoints.reserve(int(points.size()/2));
    int maxLoop = points.size()-1;

    if(isUpperPeak)
    {
        for(int i=1;i<maxLoop;++i)
        {
            if((points[i].y() > points[i-1].y()) && (points[i].y() > points[i+1].y()))
            {
                sharpPoints.append(points[i]);
            }
        }
    }
    else
    {
        for(int i=1;i<maxLoop;++i)
        {
            if((points[i].y() < points[i-1].y()) && (points[i].y() < points[i+1].y()))
            {
                sharpPoints.append(points[i]);
            }
        }
    }
}
///
/// \brief 尖峰排序
/// \param sharpPointsSorted
/// \param Points
/// \param isUpperPeak
///
void SADataProcessVectorPointF::sortPeak(QVector<QPointF> &sharpPointsSorted, const QVector<QPointF> &Points, bool isUpperPeak)
{
    getSharpPeakPoint(sharpPointsSorted,Points,isUpperPeak);
    std::sort(sharpPointsSorted.begin(),sharpPointsSorted.end(),comparePointY);
}

SADataFeatureItem* SADataProcessVectorPointF::analysisData(const QVector<QPointF>& orgPoints)
{
#ifdef _DEBUG_OUTPUT
        QElapsedTimer t;
        t.start();
#endif
    SADataFeatureItem* item = new SADataFeatureItem();
    QVector<double> y;
    getVectorPointY(orgPoints,y);
    if(orgPoints.size()<=0 || y.size()<=0)
    {
#ifdef _DEBUG_OUTPUT
        qDebug() << "orgPoints.size()<=0 || y.size()<=0 orgPoints.size()="<<orgPoints.size()
                 << "y.size():" << y.size()
                    ;
#endif
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
    int sortCount = std::min(m_sortCount,datas.size());
    QVector<QPointF> tmps;
    tmps.reserve(sortCount);

    std::copy(datas.begin(),datas.begin()+sortCount,std::back_inserter(tmps));
    item->appendItem(tr("ascending order"),tmps);//升序
    tmps.clear();
    tmps.reserve(sortCount);
    std::copy(datas.rbegin(),datas.rbegin()+sortCount,std::back_inserter(tmps));
    item->appendItem(tr("descending order"),tmps);//降序
#ifdef _DEBUG_OUTPUT
    qDebug() << "analysisData points:" << orgPoints.size() << " cost:" << t.elapsed() << " ms";
#endif
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
