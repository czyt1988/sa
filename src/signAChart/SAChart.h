#ifndef SACHART_H
#define SACHART_H
#include "SAChartGlobals.h"
#include "SASeries.h"
#include <QColor>
#include <QFont>
#include <QVector>
#include <QRectF>
#include "qwt_date.h"
#include "qwt_plot.h"
#include "qwt_symbol.h"
#include "qwt_plot_item.h"
#include <QPainterPath>
#include <QList>
#include "qwt_series_store.h"
class QwtPlotItem;
class QwtScaleDraw;
class QwtDateScaleDraw;
class QwtPlotCurve;
class QwtPlotBarChart;
///
/// \brief 这是一个辅助类，用于绘图的辅助
///
class SA_CHART_EXPORT SAChart
{
public:
    enum RTTI
    {
        RTTI_SA_SELECT_REGION_SHAPE = QwtPlotItem::Rtti_PlotUserItem + 100 ///< 区域选择
    };

    enum Value{
        Nan = -1
    };
    //更加强制的replot，就算设置为不实时刷新也能实现重绘
    void replot(QwtPlot* chart);
    //获取当前正在显示的区域
    static QRectF getVisibleRegionRang(QwtPlot* chart);
    //获取当前正在显示的区域
    static QRectF getVisibleRegionRang(QwtPlot* chart,int xAxis,int yAxis);
    //动态获取item的颜色，使用dynamic_cast,需要注意效率问题
    static QColor dynamicGetItemColor(const QwtPlotItem *item,const QColor& defaultColor = Qt::black);
////////////////////// 坐标变换相关操作//////////////////////////////
    //坐标轴数据互转（把坐标轴转换为另外一个坐标轴数据而保持屏幕位置不变）
    static QPointF transformValue(QwtPlot*chart,const QPointF& p,int orgXAxis,int orgYAxis,int otherXAxis,int otherYAxis);
    //坐标轴数据互转（把坐标轴转换为另外一个坐标轴数据而保持屏幕位置不变）
    static QPainterPath transformPath(QwtPlot*chart,const QPainterPath& p,int orgXAxis,int orgYAxis,int otherXAxis,int otherYAxis);
    //图中1像素在实际数据的偏移(正向)
    static QPointF calcOnePixelOffset(QwtPlot* chart,int xaxis,int yaxis);
////////////////////// 坐标轴相关操作//////////////////////////////

    //是否允许显示坐标轴
    static void setAxisEnable(QwtPlot*chart, int axisID, bool b);
    //设置坐标轴的标题
    static void setAxisTitle(QwtPlot*chart,int axisID,const QString& text);
    //设置坐标轴文字的字体
    static void setAxisFont(QwtPlot*chart,int axisID,const QFont &font);
    //设置坐标轴文字的旋转
    static void setAxisLabelRotation(QwtPlot*chart,int axisID,double v);
    //设置坐标轴最小刻度
    static void setAxisScaleMin(QwtPlot*chart,int axisID,double v);
    //设置坐标轴最大刻度
    static void setAxisScaleMax(QwtPlot*chart,int axisID,double v);
    //指定坐标轴端点到窗体的距离-影响坐标轴标题的显示
    static void setAxisBorderDistStart(QwtPlot*chart,int axisID,int v);
    //指定坐标轴端点到窗体的距离-影响坐标轴标题的显示
    static void setAxisBorderDistEnd(QwtPlot*chart,int axisID,int v);
    //设置坐标轴和画板的偏移距离
    static void setAxisMargin(QwtPlot*chart,int axisID,int v);
    //设置坐标轴的间隔
    static void setAxisSpacing(QwtPlot*chart,int axisID,int v);
    //设置坐标轴文字的对齐方式
    static void setAxisLabelAlignment(QwtPlot*chart,int axisID,Qt::Alignment v);
    //设置为普通线性坐标轴
    static QwtScaleDraw* setAxisNormalScale(QwtPlot *chart,int axisID);
    //设置坐标轴为时间坐标
    static QwtDateScaleDraw* setAxisDateTimeScale(QwtPlot *chart, int axisID,const QString& format,QwtDate::IntervalType type = QwtDate::Second);
    //获取时间坐标轴，若当前不是时间坐标轴，返回nullptr
    static QwtDateScaleDraw* getAxisDateTimeScale(QwtPlot *chart, int axisID);
    //获取对应坐标轴的id,如 xTop会返回xBottom
    static int otherAxis(int axisID);
////////////////////// 曲线数据相关操作//////////////////////////////
    //获取vector point的y
    static size_t getYDatas(const QVector<QPointF>& xys,QVector<double>& ys);
    //获取vector point的x
    static size_t getXDatas(const QVector<QPointF>& xys,QVector<double>& xs);
    //获取一个曲线的y值
    static size_t getYDatas(QVector<double>& ys,const QwtSeriesStore<QPointF>* cur,const QRectF& rang = QRectF());
    //获取一个曲线的x值
    static size_t getXDatas(QVector<double>& xs,const QwtSeriesStore<QPointF>* cur,const QRectF& rang = QRectF());
    //获取一个曲线的xy值
    static size_t getXYDatas(QVector<QPointF>& xys, const QwtSeriesStore<QPointF>* cur, const QRectF& rang = QRectF());
    static size_t getXYDatas(QVector<double>& xs, QVector<double>& ys,const QwtSeriesStore<QPointF>* cur, const QRectF& rang = QRectF());
    static size_t getXYDatas(QVector<QPointF>& xys, QVector<double>& xs, QVector<double>& ys,const QwtSeriesStore<QPointF>* cur, const QRectF& rang = QRectF());
    static size_t getXYDatas(QVector<QPointF>& xys, const QwtSeriesStore<QPointF>* cur, const QVector<int>& index);
    //对2d数据点的提取操作
    static size_t getXYDatas(QVector<int>& indexs,QVector<QPointF>& points, const QwtSeriesStore<QPointF>* series,const QPainterPath& rang);
    //对2d数据点的提取操作
    static size_t getXYDatas(QVector<QPointF>& points, const QwtSeriesStore<QPointF>* series,const QPainterPath& rang);
    static size_t getXYDatas(QVector<double>& xs, QVector<double>& ys, const QwtSeriesStore<QPointF>* series,const QPainterPath& rang);
    //对2d数据点的提取操作
    static size_t getXYIndex(QVector<int>& indexs, const QwtSeriesStore<QPointF>* series,const QPainterPath& rang );

    //把范围内的数据移除 返回移除的个数
    static int removeDataInRang(const QRectF &removeRang, const QVector<QPointF>& rawData, QVector<QPointF>& newData);
    static int removeDataInRang(const QPainterPath &removeRang, const QVector<QPointF>& rawData, QVector<QPointF>& newData);
    static int removeDataInRang(const QRectF& removeRang,QwtSeriesStore<QPointF>* curve);
    static int removeDataInRang(const QPainterPath& removeRang,QwtSeriesStore<QPointF>* curve);

////////////////////// QwtPlotCurve 曲线相关操作//////////////////////////////


    //设置符号
    static void setCurveSymbol(QwtPlotCurve* cur,QwtSymbol::Style style,const QSize &size = QSize(8,8));
    static void setCurveLinePenStyle(QwtPlotCurve* cur,Qt::PenStyle style);
    //设置曲线的样式
    static void setCurvePenStyle(QwtPlotCurve* cur,Qt::PenStyle style);
////////////////////// QwtPlotBarChart曲线相关操作//////////////////////////////
    //获取屏幕位置离bar最近的点，类似于QwtPlotCurve::closestPoint
    static int closestPoint(const QwtPlotBarChart* bar,const QPoint &pos, double *dist );

};

#endif // SACHART_H
