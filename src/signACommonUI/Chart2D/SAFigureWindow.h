#ifndef SAFIGUREWINDOW_H
#define SAFIGUREWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QPainter>
#include "SAMainWindow.h"
#include "SACommonUIGlobal.h"
#include "qwt_plot_histogram.h"
class QwtPlotCurve;
class QwtPlotItem;
class SAFigureOptCommand;
class SAAbstractDatas;
class SAVectorInterval;
class DataFeaturePlotInfoItem;
class SAChart2D;
class SAFigureContainer;
class CurveDataFeatureItem;
class SAFigureWindowPrivate;
class SAFigureChartRubberbandEditOverlay;
///
/// \brief SA的绘图窗口
///
class SA_COMMON_UI_EXPORT SAFigureWindow : public SAMainWindow
{
    Q_OBJECT
    SA_IMPL(SAFigureWindow)

public:
    explicit SAFigureWindow(QWidget *parent = 0);
    ~SAFigureWindow();
    QList<QWidget *> getWidgets() const;

    //添加一个2D chart
    SAChart2D *create2DPlot();
    SAChart2D *create2DSubPlot(float xPresent, float yPresent, float wPresent, float hPresent);

    //不支持redo/undo的添加窗口操作
    void _addWidget(QWidget *w, float xPresent, float yPresent, float wPresent, float hPresent);

    //获取所有的图表
    QList<SAChart2D *> get2DPlots() const;

    //获取当前的2d绘图指针
    SAChart2D *current2DPlot() const;

    //清空所有图 会连续发送chartRemoved信号
    void clearAll();

    //设置画布背景色
    void setBackgroundColor(const QBrush& brush);
    void setBackgroundColor(const QColor& clr);

    //获取窗口的位置
    QRectF getWidgetPos(QWidget *w) const;

    //设置当前的2dplot
    bool setCurrent2DPlot(SAChart2D *p);

    //通过item查找对应的SAChart2D，如果没有返回nullptr
    SAChart2D *findChartFromItem(QwtPlotItem *item);

    //开启子窗口编辑模式
    void enableSubWindowEditMode(bool enable = true, SAFigureChartRubberbandEditOverlay *ptr = nullptr);
    SAFigureChartRubberbandEditOverlay *subWindowEditModeOverlayWidget() const;

    //判断是否在进行子窗口编辑
    bool isSubWindowEditingMode() const;

    //使用支持redo/undo模式的改变子窗口大小
    void resizeWidget(QWidget *p, const QRect& newRect, const QRect& oldRect = QRect());

    //添加命令
    void appendCommand(SAFigureOptCommand *cmd);

    //清空命令
    void clearUndoCommand();

    //判断命令是否是空的
    bool isUndoCommandClean() const;

    //返回当前光标下的widget
    QWidget *cursorWidget() const;

    //返回在当前光标下的2D图
    SAChart2D *cursor2DChart() const;

public slots:
    //redo
    void redo();

    //undo
    void undo();

    //清空回退栈
    void clearRedoUndoStack();

#if 0
protected:
    void keyPressEvent(QKeyEvent *e);
#endif

signals:
    //添加了一个绘图发送的信号
    void chartAdded(QwtPlot *plot);

    //删除了一个绘图发送的信号
    void chartWillRemove(QwtPlot *plot);

    //当前选中的发生改变
    void currentWidgetChanged(QWidget *w);
};

SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream& out, const SAFigureWindow *p);
SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream& in, SAFigureWindow *p);

#endif // SAFIGUREWINDOW_H
