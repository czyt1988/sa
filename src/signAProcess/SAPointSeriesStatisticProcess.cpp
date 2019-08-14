#include "SAPointSeriesStatisticProcess.h"
#include "czyQArray.h"

class SAPointSeriesStatisticProcessPrivate
{
    SA_IMPL_PUBLIC(SAPointSeriesStatisticProcess)
public:
    SAPointSeriesStatisticProcessPrivate(SAPointSeriesStatisticProcess* p):q_ptr(p)
    {

    }
    ~SAPointSeriesStatisticProcessPrivate()
    {

    }
public:
    uint mSortCount;
    QVector<QPointF> mPoints;
    SAVariantHashData mArgs;
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
    //emit result();
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
 * @param sortCount 排序个数
 */
void SAPointSeriesStatisticProcess::setSortCount(uint sortCount)
{
    d_ptr->mSortCount = sortCount;
}

/**
 * @brief 设置参数，系列点
 * @param points 点集
 * @param args 传参
 * @param key 标识
 */
void SAPointSeriesStatisticProcess::setPoints(const QVector<QPointF> &points
                                              , const SAVariantHashData &args)
{
    d_ptr->mPoints = points;
    d_ptr->mArgs = args;
}
/**
 * @brief 把vector points的y值提取
 * @param points 传入的vector point
 * @param ys 获取到的y值
 */
void SAPointSeriesStatisticProcess::getVectorPointY(const QVector<QPointF> &points
                                                    , QVector<double> &ys)
{
    czy::QArray::get_qvectorpointf_y(points,ys.begin());
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
        czy::Array::find_upper_sharp_peak(points.begin(),points.end()
                                          ,std::back_inserter(sharpPoints)
                                          ,[](const QPointF& a,const QPointF& b)->bool{
            return a.y() > b.y();
        });
    }
    else
    {
        czy::Array::find_lower_sharp_peak(points.begin(),points.end()
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
