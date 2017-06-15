#ifndef SACHART2D_H
#define SACHART2D_H
#include "SA2DGraph.h"
#include "SACommonUIGlobal.h"
class SAAbstractDatas;
class SAXYSeries;
class SA_COMMON_UI_EXPORT SAChart2D : public SA2DGraph
{
    Q_OBJECT
public:
    SAChart2D(QWidget *parent = nullptr);
    ~SAChart2D();
    //添加曲线
    SAXYSeries *addCurve(SAAbstractDatas* datas);
    SAXYSeries* addCurve(SAAbstractDatas* datas,double xStart,double xDetal);
    SAXYSeries* addCurve(SAAbstractDatas* x,SAAbstractDatas* y,const QString& name = QString());
    //from SA2DGraph
    QwtPlotCurve* addCurve(const QVector<QPointF>& xyDatas);
    QwtPlotCurve* addCurve(const double *xData, const double *yData, int size);
    QwtPlotCurve* addCurve(const QVector< double > &xData, const QVector< double > &yData);
    void addCurve(QwtPlotCurve* cur);
};

#endif // SACHART2D_H
