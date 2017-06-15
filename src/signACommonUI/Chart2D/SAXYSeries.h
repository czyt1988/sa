#ifndef SAXYSERIES_H
#define SAXYSERIES_H
#include "SACommonUIGlobal.h"
#include "qwt_plot_curve.h"
#include <QSet>
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SAXYSeries : public QwtPlotCurve
{
public:
    SAXYSeries();
    SAXYSeries(const QString &title);
    SAXYSeries(const QString &title,SAAbstractDatas* dataPoints);
    //加载数据
    bool setSamples(SAAbstractDatas* dataPoints);
    bool setSamples(SAAbstractDatas* x,SAAbstractDatas* y);
    bool setSamples(SAAbstractDatas* y,double xStart,double xDetal);
    //获取此曲线相关的数据
    QSet<SAAbstractDatas *> linkDatas() const;
    //判断此曲线是否和某数据相关
    bool isContainData(SAAbstractDatas* d);
    //
    void removeLinkDataPtr(const SAAbstractDatas* ptr);
private:
    QSet<SAAbstractDatas*> m_linkDatas;
};



#endif // SAXYSERIES_H
