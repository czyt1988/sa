#include "SAPointSeriesStatisticProcess.h"
#include "czyQArray.h"
#include "czyMath.h"
#include <algorithm>
#include "SAXMLTagDefined.h"
#include "SAXMLWriteHelper.h"



class SAPointSeriesStatisticProcessPrivate
{
    SA_IMPL_PUBLIC(SAPointSeriesStatisticProcess)
public:
    SAPointSeriesStatisticProcessPrivate(SAPointSeriesStatisticProcess* p):q_ptr(p)
    ,mSortCount(10)
    {

    }
    ~SAPointSeriesStatisticProcessPrivate()
    {

    }
public:
    uint mSortCount;
    QVector<QPointF> mPoints;
    QVariantHash mArgs;
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

    czy::Math::get_statistics(y.begin(),y.end()
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
    //
    SAXMLWriteHelper xmlHelper(ATT_SA_TYPE_VPFR);
    for(auto i = d_ptr->mArgs.begin();i != d_ptr->mArgs.end();++i)
    {
        xmlHelper.writeHeadValue(i.key(),i.value());
    }
    xmlHelper.startContentWriteGroup("param");
    xmlHelper.writeContentValue("sum",sum);
    xmlHelper.writeContentValue("min",min);
    xmlHelper.writeContentValue("max",max);
    xmlHelper.writeContentValue("mid",mid);
    xmlHelper.writeContentValue("mean",mean);
    xmlHelper.writeContentValue("var",var);
    xmlHelper.writeContentValue("stdVar",stdVar);
    xmlHelper.writeContentValue("skewness",skewness);
    xmlHelper.writeContentValue("kurtosis",kurtosis);
    xmlHelper.writeContentValue("peak2peak",peak2peak);
    xmlHelper.writeContentValue("minPoint",minPoint);
    xmlHelper.writeContentValue("maxPoint",maxPoint);
    xmlHelper.writeContentValue("midPoint",midPoint);
    xmlHelper.endContentWriteGroup();

    int sortCount = std::min(d_ptr->mSortCount,(uint)datas.size());
    const int datasSize = datas.size();
    xmlHelper.startContentWriteGroup("top");
    for(int i=0;i<sortCount;++i)
    {
        QPointF top = datas[datasSize-i-1];
        xmlHelper.writeContentValue(QString("top:%1").arg(i+1),top);
    }
    xmlHelper.endContentWriteGroup();

    xmlHelper.startContentWriteGroup("bottom");
    for(int i=0;i<sortCount;++i)
    {
        QPointF top = datas[i];
        xmlHelper.writeContentValue(QString("bottom:%1").arg(i+1),top);
    }
    xmlHelper.endContentWriteGroup();
    emit result(xmlHelper.toString(),getID(),SAAbstractProcess::XmlString);
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
void SAPointSeriesStatisticProcess::setPoints(const QVector<QPointF> &points)
{
    d_ptr->mPoints = points;
}

void SAPointSeriesStatisticProcess::setArgs(const QVariantHash &args)
{
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
