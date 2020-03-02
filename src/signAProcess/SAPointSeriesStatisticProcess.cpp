#include "SAPointSeriesStatisticProcess.h"
#include "czyMath.h"
#include <algorithm>
#include "SASeriesAlgorithm.h"
#include "SAQtSeriesAlgorithm.h"
#include <QDebug>
#define IS_DEBUG_PRINT 1

class SAPointSeriesStatisticProcessPrivate
{
    SA_IMPL_PUBLIC(SAPointSeriesStatisticProcess)
public:
    SAPointSeriesStatisticProcessPrivate(SAPointSeriesStatisticProcess* p):q_ptr(p)
    ,mSortCount(50)
    {

    }
    ~SAPointSeriesStatisticProcessPrivate()
    {

    }
public:
    uint mSortCount;
    QVector<QPointF> mPoints;
};

SAPointSeriesStatisticProcess::SAPointSeriesStatisticProcess(QObject *par):SAAbstractProcess(par)
  ,d_ptr(new SAPointSeriesStatisticProcessPrivate(this))
{

}

SAPointSeriesStatisticProcess::~SAPointSeriesStatisticProcess()
{

}

void SAPointSeriesStatisticProcess::run()
{
    QVector<double> y;
    getVectorPointY(d_ptr->mPoints,y);
    if(d_ptr->mPoints.size()<=0 || y.size()<=0)
    {
#if IS_DEBUG_PRINT
    qDebug() << "invalid datas";
#endif
        emit finish(getID());
        return;
    }
    QVector<QPointF> datas = d_ptr->mPoints;
    double sum;
    double mean;
    double var;
    double stdVar;
    double skewness;
    double kurtosis;
    size_t n = datas.size();
    SA::Math::get_statistics(y.begin(),y.end()
                              ,sum
                              ,mean
                              ,var
                              ,stdVar
                              ,skewness
                              ,kurtosis);
    std::sort(datas.begin(),datas.end()
              ,[](const QPointF& a,const QPointF& b)->bool{
        return (a.y() < b.y());
    });
    double min = datas.cbegin()->y();//最小
    double max = (datas.cend()-1)->y();//最大
    double mid = n>1 ? (datas.cbegin() + int(n/2))->y() : min;//中位数
    double peak2peak = max - min;
    QPointF minPoint = *datas.cbegin();
    QPointF maxPoint = *(datas.cend()-1);
    QPointF midPoint = n>1 ? *(datas.cbegin() + int(n/2)) : minPoint;//中位数
    QVariantHash res;
    res["sum"] = sum;
    res["mean"] = mean;
    res["var"] = var;
    res["std-var"] = stdVar;
    res["skewness"] = skewness;
    res["kurtosis"] = kurtosis;
    res["min"] = min;
    res["min"] = max;
    res["mid"] = mid;
    res["peak-peak"] = peak2peak;
    res["min-point"] = minPoint;
    res["min-point"] = maxPoint;
    res["min-point"] = midPoint;
    int sortedcount = d_ptr->mSortCount < n ? d_ptr->mSortCount : n;
    QVector<QPointF> tops(sortedcount);
    QVector<QPointF> lows(sortedcount);
    //拷贝tops
    std::copy(datas.rbegin(),datas.rbegin()+sortedcount,tops.begin());
    //拷贝lows
    std::copy(datas.begin(),datas.begin()+sortedcount,lows.begin());
    res["sorted-tops"] = QVariant::fromValue(tops);
    res["sorted-lows"] = QVariant::fromValue(lows);
    emit result(res,getID());
    emit finish(getID());
}
/**
 * @brief 获取返回的排序的个数,默认为100
 * @return 排序的个数
 */
uint SAPointSeriesStatisticProcess::getSortCount() const
{
    return d_ptr->mSortCount;
}
/**
 * @brief 设置排序的个数，返回的内容排序的个数包含的长度，此值默认100，这个设置为了避免返回内容过多
 * 对应参数描述：ret-sorted-count
 * @param sortCount 排序个数
 */
void SAPointSeriesStatisticProcess::setSortCount(uint sortCount)
{
    d_ptr->mSortCount = sortCount;
}

/**
 * @brief 设置参数，系列点
 * 对应参数描述：points
 * @param points 点集
 */
void SAPointSeriesStatisticProcess::setPoints(const QVector<QPointF> &points)
{
    d_ptr->mPoints = points;
}



QString SAPointSeriesStatisticProcess::getName() const
{
   return "sa_point_series_statistic_process";
}

QString SAPointSeriesStatisticProcess::getNameSpace() const
{
    return "sa";
}

bool SAPointSeriesStatisticProcess::setArgs(const QVariantHash &args)
{
   if(!args.contains("points"))
   {
       return false;
   }
   const QVariant& v0 = args["points"];
   if(!v0.canConvert<QVector<QPointF>>())
   {
       return false;
   }
   setPoints(v0.value<QVector<QPointF>>());
   //获取其他参数
   if(args.contains("ret-sorted-count"))
   {
       const QVariant& v = args["ret-sorted-count"];
       if(!v.canConvert<int>())
       {
           return false;
       }
       setSortCount(v.value<int>());
   }
   return true;
}
/**
 * @brief 把vector points的y值提取
 * @param points 传入的vector point
 * @param ys 获取到的y值
 */
void SAPointSeriesStatisticProcess::getVectorPointY(const QVector<QPointF> &points
                                                    , QVector<double> &ys)
{
    if(ys.size() < points.size())
    {
        ys.resize(points.size());
    }
    SA::get_qvectorpointf_y(points,ys.begin());
}
/**
 * @brief 获取尖峰的点 - 所谓尖峰是指三点a,b,c b>a && b>c 就说明b是尖峰
 * @param sharpPoints 尖峰值引用
 * @param Points 需要获得的数据
 * @param isUpperPeak 获取的是上峰值
 */
void SAPointSeriesStatisticProcess::getSharpPeakPoint(QVector<QPointF> &sharpPoints
                                                      , const QVector<QPointF> &points
                                                      , bool isUpperPeak)
{
    sharpPoints.clear();
    sharpPoints.reserve(int(points.size()/2));
    if(isUpperPeak)
    {
        SA::find_upper_sharp_peak(points.begin(),points.end()
                                          ,std::back_inserter(sharpPoints)
                                          ,[](const QPointF& a,const QPointF& b)->bool{
            return a.y() > b.y();
        });
    }
    else
    {
        SA::find_lower_sharp_peak(points.begin(),points.end()
                                          ,std::back_inserter(sharpPoints)
                                          ,[](const QPointF& a,const QPointF& b)->bool{
            return a.y() < b.y();
        });
    }
}

/**
 * @brief 两个点对y值进行大小比较
 * @param a
 * @param b
 * @return
 */
int SAPointSeriesStatisticProcess::comparePointY(const QPointF &a, const QPointF &b)
{
    return int( a.y() < b.y() );
}
/**
 * @brief 尖峰排序
 * @param sharpPointsSorted
 * @param Points
 * @param isUpperPeak
 */
void SAPointSeriesStatisticProcess::sortPeak(QVector<QPointF> &sharpPointsSorted, const QVector<QPointF> &Points, bool isUpperPeak)
{
    getSharpPeakPoint(sharpPointsSorted,Points,isUpperPeak);
    std::sort(sharpPointsSorted.begin(),sharpPointsSorted.end(),comparePointY);

}
