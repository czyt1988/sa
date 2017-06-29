#ifndef SADATAPROCESSVECTORPOINTF_H
#define SADATAPROCESSVECTORPOINTF_H
#include "SADataFeatureItem.h"
#include <QObject>
#include <QVector>
#include <QPointF>
class SADataProcessVectorPointF : public QObject
{
    Q_OBJECT
public:
    SADataProcessVectorPointF(QObject* parent);
    int getSortCount() const;
    void setSortCount(int sortCount);

public slots:
    //设置需要计算的点，并开始计算
    void setPoints(const QVector<QPointF>& points);
    //从点集获取y值
    static void getVectorPointY(const QVector<QPointF>& points, QVector<double>& ys);

private:
    SADataFeatureItem* analysisData();
signals:
    void result(const SADataFeatureItem* result);
private:
    QVector<QPointF>& m_points;///< 记录点集
    int m_sortCount;///< 记录要显示的排序条目
};

#endif // SADATAPROCESS_H
