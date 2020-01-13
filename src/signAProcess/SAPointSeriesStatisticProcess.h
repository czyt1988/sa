#ifndef SAPOINTSERIESSTATISTICPROCESS_H
#define SAPOINTSERIESSTATISTICPROCESS_H
#include "SAAbstractProcess.h"
#include <QPointF>
#include <QVariantHash>
class SAPointSeriesStatisticProcessPrivate;
/**
 * @brief 对点集的计算和统计
 *
 * 此类用于对点集进行统计分析
 * 设置参数使用：setArgs @sa setArgs 参数说明：
 * - "points" type:QVector<QPointF> 输入的点序列
 * - "ret-sorted-count" type:int 定义返回排序的数量，默认50，可选参数
 *
 * 返回参数通过信号result @sa SAAbstractProcess::result
 * 返回说明：
 * - "sum" -> type:double sum
 * - "mean" -> type:double mean
 * - "var" -> type:double var
 * - "std-var" -> type:double stdVar
 * - "skewness" -> type:double skewness
 * - "kurtosis" -> type:double kurtosis
 * - "min" -> type:double min
 * - "min" -> type:double max
 * - "mid" -> type:double mid
 * - "peak-peak" -> type:double peak2peak
 * - "min-point" -> type:double minPoint
 * - "min-point" -> type:double maxPoint
 * - "min-point" -> type:double midPoint
 * - "sorted-tops" -> type:QVector<QPointf> tops 按顺序最大的n个
 * - "sorted-lows" -> type:QVector<QPointf> lows 按顺序最小的n个
 * @author czy
 * @date 2019-08-08
 */
class SA_PROCESS_EXPORT SAPointSeriesStatisticProcess : public SAAbstractProcess
{
    Q_OBJECT
    SA_IMPL(SAPointSeriesStatisticProcess)
public:
    SAPointSeriesStatisticProcess(QObject* par=nullptr);
    virtual ~SAPointSeriesStatisticProcess();
    virtual void run();
    //设置返回的排序个数
    uint getSortCount() const;
    //设置需要计算的点 对应参数描述：points
    void setPoints(const QVector<QPointF>& points);
    //设置排序的个数 对应参数描述：ret-sorted-count
    void setSortCount(uint sortCount);
    virtual QString getName() const override;
    virtual QString getNameSpace() const override;
    /**
     * @brief setArgs 设置参数
     * @param args 参数说明：
     * - "points" type:QVector<QPointF> 输入的点序列
     * - "ret-sorted-count" type:int 定义返回排序的数量，默认50，可选参数
     * @return
     */
    virtual bool setArgs(const QVariantHash& args) override;
public:
    //从点集获取y值
    static void getVectorPointY(const QVector<QPointF>& points, QVector<double>& ys);
    //获取尖峰的点 - 所谓尖峰是指三点a,b,c b>a && b>c 就说明b是尖峰
    static void getSharpPeakPoint(QVector<QPointF>& sharpPoints, const QVector<QPointF>& points, bool isUpperPeak = true);
    //比较点的y值
    static int comparePointY(const QPointF& a,const QPointF& b);
    //峰值排序
    static void sortPeak(QVector<QPointF>& sharpPointsSorted,const QVector<QPointF>& Points,bool isUpperPeak = true);
};


#endif // SAPOINTSERIESSTATISTICPROCESS_H
