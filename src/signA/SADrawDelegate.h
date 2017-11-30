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
///
/// \brief 处理绘图函数
///
class SADrawDelegate : public SAMainWindowDelegate
{
public:
    SADrawDelegate(MainWindow* wnd);
    virtual ~SADrawDelegate();
    //绘图
    QList<QwtPlotCurve *> drawLineChart(const QList<SAAbstractDatas*>& datas);
    QwtPlotCurve* drawLineChart(SAAbstractDatas* data,SAChart2D *chart);
    QwtPlotCurve* drawLineChart(SAAbstractDatas* x,SAAbstractDatas* y,QString name,SAChart2D *chart);
    //棒图
    QwtPlotHistogram* drawBar(SAAbstractDatas* data);
    QList<QwtPlotHistogram*> drawBar(const QList<SAAbstractDatas*>& datas);
    //散点图
    QList<QwtPlotCurve *> drawScatterChart(const QList<SAAbstractDatas*>& datas);
    //创建绘图窗口
    SAMdiSubWindow* createFigureMdiSubWidget(const QString& title = QString());
    //把QMdiSubWindow的内部SAFigureWidget获取
    static SAFigureWindow* getFigureWidgetFromMdiSubWindow(QMdiSubWindow* w);
protected:
    QwtPlotCurve* drawLineChart(SAChart2D *chart , SAVectorPointF* points);
private:
    unsigned int m_nUserChartCount;
};

#endif // SADRAWDELEGATE_H
