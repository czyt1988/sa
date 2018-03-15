#ifndef SAFIGUREWINDOW_H
#define SAFIGUREWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include "SAAbstractFigure.h"
#include "SACommonUIGlobal.h"

#include "qwt_plot_histogram.h"
class QwtPlotCurve;
class QwtPlotItem;

class SAAbstractDatas;
class SAVectorInterval;
class DataFeaturePlotInfoItem;
class SAChart2D;
class SAFigureContainer;
class CurveDataFeatureItem;
class SAFigureWindowPrivate;

///
/// \brief SA的绘图窗口
///
class SA_COMMON_UI_EXPORT SAFigureWindow : public QMainWindow
        ,public SAAbstractFigure
{
    Q_OBJECT
    SA_IMPL(SAFigureWindow)
public:
    explicit SAFigureWindow(QWidget *parent = 0);
    ~SAFigureWindow();
    //添加一个2D chart
    SAChart2D* create2DPlot();
    SAChart2D* create2DSubPlot(float xPresent, float yPresent, float wPresent, float hPresent);
    //获取所有的图表
    QList<SAChart2D*> get2DPlots() const;
    //获取当前的2d绘图指针
    SAChart2D* current2DPlot() const;
    //清空所有图 会连续发送chartRemoved信号
    void clearAll();
    //设置画布背景色
    void setBackgroundColor(const QBrush& brush);
    void setBackgroundColor(const QColor& clr);
    //获取窗口的位置
    QRectF getWidgetPos(QWidget *w) const;
    //设置当前的2dplot
    void setCurrent2DPlot(SAChart2D* p);
    //通过item查找对应的SAChart2D，如果没有返回nullptr
    SAChart2D *findChartFromItem(QwtPlotItem *item);
protected:
public slots:
    void redo();
    void undo();
#if 0
protected:
    void keyPressEvent(QKeyEvent *e);
#endif
signals:
    //添加了一个绘图发送的信号
    void chartAdded(QwtPlot* plot);
    //删除了一个绘图发送的信号
    void chartRemoved(QwtPlot* plot);
};

SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SAFigureWindow* p);
SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SAFigureWindow* p);
#endif // SAFIGUREWINDOW_H
