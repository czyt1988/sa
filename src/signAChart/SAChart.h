#ifndef SACHART_H
#define SACHART_H
#include "SAChartGlobals.h"
#include "SASeries.h"
#include <QColor>
#include <QFont>
#include "qwt_date.h"
#include "qwt_plot.h"
class QwtPlotItem;
class QwtScaleDraw;
class QwtDateScaleDraw;

///
/// \brief 这是一个辅助类，用于绘图的辅助
///
class SA_CHART_EXPORT SAChart
{
public:
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

    static void setAxisEnable(QwtPlot*chart, int axisID, bool b);
    static void setAxisTitle(QwtPlot*chart,int axisID,const QString& text);
    static void setAxisFont(QwtPlot*chart,int axisID,const QFont &font);
    static void setAxisLabelRotation(QwtPlot*chart,int axisID,double v);
    static void setAxisScaleMin(QwtPlot*chart,int axisID,double v);
    static void setAxisScaleMax(QwtPlot*chart,int axisID,double v);
    static void setAxisBorderDistStart(QwtPlot*chart,int axisID,int v);
    static void setAxisBorderDistEnd(QwtPlot*chart,int axisID,int v);
    static void setAxisMargin(QwtPlot*chart,int axisID,int v);
    static void setAxisSpacing(QwtPlot*chart,int axisID,int v);
    static void setAxisLabelAlignment(QwtPlot*chart,int axisID,Qt::Alignment v);
    static QwtScaleDraw* setAxisNormalScale(QwtPlot *chart,int axisID);
    static QwtDateScaleDraw* setAxisDateTimeScale(QwtPlot *chart, int axisID);
    static QwtDateScaleDraw* setAxisDateTimeScale(QwtPlot *chart, int axisID,const QString& format,QwtDate::IntervalType type = QwtDate::Second);
    static QwtDateScaleDraw* getAxisDateTimeScale(QwtPlot *chart, int axisID);
    static int otherAxis(int axisID);
};

#endif // SACHART_H
