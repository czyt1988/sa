#ifndef SAPOINTSERIESSTATISTICPROCESS_H
#define SAPOINTSERIESSTATISTICPROCESS_H
#include "SAAbstractProcess.h"
#include <QPointF>
class SAPointSeriesStatisticProcessPrivate;
/**
 * @brief 对点集的计算和统计
 *
 * 此类用于对点集进行统计分析
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
    void setSortCount(uint sortCount);
    //设置需要计算的点
    void setPoints(const QVector<QPointF>& points);
    void setArgs(const QVariantHash& args);
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
