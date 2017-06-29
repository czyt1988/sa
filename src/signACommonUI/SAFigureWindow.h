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

///
/// \brief SA的绘图窗口
///
class SA_COMMON_UI_EXPORT SAFigureWindow : public QMainWindow
        ,public SAAbstractFigure
{
    Q_OBJECT
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

    //设置画布背景色
    void setBackgroundColor(const QBrush& brush);
    void setBackgroundColor(const QColor& clr);
    //
    //============
 //不满足单一职责原则
    //============

signals:
    //绘图数据变更消息
    void chartDataChanged(QwtPlot* plot,QwtPlotItem* item);
private:
    void initUI();
private:
    class RealPlotItem{
    public:
        void append(QwtPlotItem* item);
    private:
        QList<QwtPlotItem*> m_plotItem;
    };

    class UI
    {
    public:
        void setupUI(SAFigureWindow* par);
        void retranslateUi(SAFigureWindow* par);
        SAFigureContainer *centralwidget;
    };
protected:
    //记忆关系
    void rememberRelationShip(QwtPlotItem* cur,const QList<SAAbstractDatas*>& datas);
protected:
    RealPlotItem m_realPlotItem; ///< 记录真正绘制的图像，对于一些标记线就不会进入这个list里，且是addCurve函数调用时才会加入这个list
    QMap<QwtPlotItem*,QList<SAAbstractDatas*> > m_relationShip;
private:
    QScopedPointer<UI> ui;
    QwtPlotSeriesItem* m_lastPlotItem;
    SAChart2D *m_currentPlot;
};

#endif // SAFIGUREWINDOW_H
