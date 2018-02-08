#ifndef SADRAWDELEGATE_H
#define SADRAWDELEGATE_H
#include <SAMainWindowDelegate.h>

class SAMdiSubWindow;
class MainWindow;
class SAFigureWindow;
class QwtPlotCurve;
//class SAPlotChart;
class QwtPlotHistogram;
class SAAbstractDatas;
class SAVectorPointF;
class QMdiSubWindow;
class SAChart2D;
class QwtPlotTradingCurve;
class QwtPlotBarChart;
///
/// \brief 处理绘图函数
///
class SADrawDelegate : public SAMainWindowDelegate
{
public:
    SADrawDelegate(MainWindow* wnd);
    virtual ~SADrawDelegate();
    //绘图
    QList<QwtPlotCurve *> drawLine(const QList<SAAbstractDatas*>& datas);
    QList<QwtPlotCurve *> drawLine(const QList<SAAbstractDatas*>& datas,SAChart2D* chart);
    QwtPlotCurve* drawLine(SAAbstractDatas* data,SAChart2D *chart);
    QwtPlotCurve* drawLine(SAAbstractDatas* x,SAAbstractDatas* y,QString name,SAChart2D *chart);
    //统计图
    QwtPlotHistogram* drawHistogram(SAAbstractDatas* data);
    QList<QwtPlotHistogram*> drawHistogram(const QList<SAAbstractDatas*>& datas);
    //棒图
    QList<QwtPlotBarChart*> drawBar(const QList<SAAbstractDatas*>& datas);
    //散点图
    QList<QwtPlotCurve *> drawScatter(const QList<SAAbstractDatas*>& datas);
    //箱盒图
    QList<QwtPlotTradingCurve *> drawBoxChart(const QList<SAAbstractDatas*>& datas);
    //创建绘图窗口
    SAMdiSubWindow* createFigureMdiSubWidget(const QString& title = QString());
    //把QMdiSubWindow的内部SAFigureWidget获取
    static SAFigureWindow* getFigureWidgetFromMdiSubWindow(QMdiSubWindow* w);
protected:
    QwtPlotCurve* drawLine(SAChart2D *chart , SAVectorPointF* points);
private:
    unsigned int m_nUserChartCount;
};

#endif // SADRAWDELEGATE_H
