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
class QwtPlotItem;
class QwtScaleDraw;
class QwtDateScaleDraw;
class QwtPlotCurve;
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
    //获取曲线相关的items
    static QwtPlotItemList getCurveItemList(QwtPlot* chart);
    //判断是否是关于曲线的item
    static bool isPlotCurveItem(QwtPlotItem* item);
    //获取item的颜色,无法获取单一颜色就返回QColor()
    static QColor getItemColor(QwtPlotItem* item);
    //获取item的数据个数，-1为nan
    static int getItemDataSize(QwtPlotItem* item);
    //更加强制的replot，就算设置为不实时刷新也能实现重绘
    void replot(QwtPlot* chart);
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


    ////////////////////// 曲线相关操作//////////////////////////////
    //获取vector point的y
    static size_t getYDatas(const QVector<QPointF>& xys,QVector<double>& ys);
    //获取vector point的x
    static size_t getXDatas(const QVector<QPointF>& xys,QVector<double>& xs);
    //获取一个曲线的y值
    static size_t getYDatas(QVector<double>& ys,const QwtPlotCurve* cur,const QRectF& rang = QRectF());
    //获取一个曲线的x值
    static size_t getXDatas(QVector<double>& xs,const QwtPlotCurve* cur,const QRectF& rang = QRectF());
    //获取一个曲线的xy值
    static size_t getXYDatas(QVector<QPointF>& xys, const QwtPlotCurve* cur, const QRectF& rang = QRectF());
    static size_t getXYDatas(QVector<double>& xs, QVector<double>& ys,const QwtPlotCurve* cur, const QRectF& rang = QRectF());
    static size_t getXYDatas(QVector<QPointF>& xys, QVector<double>& xs, QVector<double>& ys,const QwtPlotCurve* cur, const QRectF& rang = QRectF());

    //设置符号
    static void setCurveSymbol(QwtPlotCurve* cur,QwtSymbol::Style style,const QSize &size = QSize(8,8));
    static void setCurveLinePenStyle(QwtPlotCurve* cur,Qt::PenStyle style);
    //设置曲线的样式
    static void setCurvePenStyle(QwtPlotCurve* cur,Qt::PenStyle style);
    //把范围内的数据移除 返回移除的个数
    static int removeDataInRang(const QRectF& removeRang,QwtPlotCurve* curve);
    static int removeDataInRang(const QPainterPath& removeRang,QwtPlotCurve* curve);
    //获取选择范围里的数据索引
    static int getCurveInSelectRangIndex(const QPainterPath& rang, const QwtPlotCurve* curve, QVector<int>& indexs);
    static int getCurveInSelectRangDataAndIndex(const QPainterPath& rang, const QwtPlotCurve* curve, QVector<int>& indexs,QVector<QPointF>& points);
    static int getCurveInSelectRangData(const QPainterPath& rang, const QwtPlotCurve* curve,QVector<QPointF>& points);
    //获取范围内的数据 返回获取的个数
    static int getDataInRang(const QPainterPath& rang,QwtPlotCurve* curve,QVector<QPointF>& res);
    //获取当前正在显示的区域
    static QRectF getVisibleRegionRang(QwtPlot* chart);
    //获取当前正在显示的区域
    static QRectF getVisibleRegionRang(QwtPlot* chart,int xAxis,int yAxis);
    //获取当前显示区域的数据
    static QPoint getXInVisibleRegionDatas(QwtPlot* chart,QwtPlotCurve* cur,QVector<QPointF>& out_xys);
};

#endif // SACHART_H
