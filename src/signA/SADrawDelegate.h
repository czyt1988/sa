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
class SADrawDelegate : public SAMainWindowDelegate
{
public:
    SADrawDelegate(MainWindow* wnd);
    virtual ~SADrawDelegate();
    virtual void drawTrend();
    //绘图
    QwtPlotCurve* draw(SAAbstractDatas* data);
    QwtPlotCurve* draw(SAAbstractDatas* x,SAAbstractDatas* y,QString name);
    QwtPlotHistogram* drawBar(SAAbstractDatas* barSeries);
    //标记
    //最小二乘回归分析
    //void markLeastSquarePolyfit(int n);
    //创建绘图窗口
    SAMdiSubWindow* createFigureMdiSubWidget(const QString& title);
    //把QMdiSubWindow的内部SAFigureWidget获取
    static SAFigureWindow* getFigureWidgetFromMdiSubWindow(QMdiSubWindow* w);
protected:
    QwtPlotCurve* drawVPoint(SAFigureWindow* chart, SAVectorPointF* points);
private:
    unsigned int m_nUserChartCount;
};

#endif // SADRAWDELEGATE_H
