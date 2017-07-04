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
    SADataProcessVectorPointF(QObject* parent = nullptr);
    int getSortCount() const;
    void setSortCount(int sortCount);
public slots:
    //设置需要计算的点，并开始计算
    void setPoints(const QVector<QPointF>& points, quintptr widget, quintptr fig, quintptr item);
    //从点集获取y值
    static void getVectorPointY(const QVector<QPointF>& points, QVector<double>& ys);

private:
    SADataFeatureItem* analysisData(const QVector<QPointF> &orgPoints);
signals:
    ///
    /// \brief 返回的结果指针如果不用必须delete
    /// \param result
    /// \param widget 标记1
    /// \param item 标记2
    ///
    void result(SADataFeatureItem* result,quintptr widget,quintptr fig,quintptr item);
private:
    int m_sortCount;///< 记录要显示的排序条目
};

#endif // SADATAPROCESS_H
