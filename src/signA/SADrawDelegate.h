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
    //创建figure
    std::tuple<SAChart2D*,SAFigureWindow*,QMdiSubWindow*> createFigure(const QString& title = QString());
    //创建绘图窗口
    QMdiSubWindow *createFigureMdiSubWidget(const QString& title = QString());
    QMdiSubWindow* createFigureMdiSubWidget(SAFigureWindow* fig, const QString& title = QString());
    //绘图
    QList<QwtPlotCurve *> drawLine(const QList<SAAbstractDatas*>& datas);
    QList<QwtPlotCurve *> drawLine(const QList<SAAbstractDatas*>& datas,SAChart2D* chart);
    //添加趋势图，x轴通过xStart和xDetal自动计算
    QwtPlotCurve* drawLine(SAAbstractDatas* data, double xStart, double xDetal, SAChart2D *chart, const QString &name);
    //添加x,y曲线 chart 为nullptr时会新创建一个figure并绘图
    QwtPlotCurve* drawLine(SAAbstractDatas* x, SAAbstractDatas* y, SAChart2D *chart, const QString &name);
    //添加x,y曲线
    QwtPlotCurve* drawLine(const QVector< double > &xData, const QVector< double > &yData, SAChart2D *chart, const QString &name);
    QList<QwtPlotCurve *> drawLineWithWizard();
    //统计图
    QwtPlotHistogram* drawHistogram(SAAbstractDatas* data);
    QList<QwtPlotHistogram*> drawHistogram(const QList<SAAbstractDatas*>& datas);
    //棒图
    QList<QwtPlotBarChart*> drawBar(const QList<SAAbstractDatas*>& datas);
    //散点图
    QList<QwtPlotCurve *> drawScatter(const QList<SAAbstractDatas*>& datas);
    //箱盒图
    QList<QwtPlotTradingCurve *> drawBoxChart(const QList<SAAbstractDatas*>& datas);
    //把QMdiSubWindow的内部SAFigureWidget获取
    static SAFigureWindow* getFigureWidgetFromMdiSubWindow(QMdiSubWindow* w);
    //获取当前可用的绘图
    SAChart2D* getCurSubWindowChart() const;
    //获取当前的fig窗口
    SAFigureWindow* getCurrentFigureWindow() const;
protected:
    QwtPlotCurve* drawLine(SAChart2D *chart , SAVectorPointF* points);
private:

};

#endif // SADRAWDELEGATE_H
