#include "SAChart.h"
#include "SASeries.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_barchart.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#include "qwt_scale_draw.h"
#include "qwt_date_scale_engine.h"
#include "qwt_date_scale_draw.h"
#include "qwt_plot_multi_barchart.h"
#include <numeric>
///
/// \brief item的类型判断，cureve bar 等绘图相关返回true
/// \param item
/// \return cureve bar 等绘图相关返回true
///
QwtPlotItemList SAChart::getCurveItemList(QwtPlot *chart)
{
    const QwtPlotItemList& items = chart->itemList();
    QwtPlotItemList res;
    for(int i=0;i<items.size();++i)
    {
        if(isPlotCurveItem(items[i]))
        {
            res.append(items[i]);
        }
    }
    return res;
}

bool SAChart::isPlotCurveItem(QwtPlotItem *item)
{
    switch(item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    case QwtPlotItem::Rtti_PlotHistogram:
    case QwtPlotItem::Rtti_PlotSpectrogram:
    case QwtPlotItem::Rtti_PlotTradingCurve:
    case QwtPlotItem::Rtti_PlotBarChart:
    case QwtPlotItem::Rtti_PlotMultiBarChart:
        return true;
    default:
        return false;
    }
    return false;
}
///
/// \brief 获取item的颜色,无法获取单一颜色就返回QColor()
/// \param item
/// \return
///
QColor SAChart::getItemColor(QwtPlotItem *item)
{
    switch (item->rtti()) {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        QwtPlotCurve* p = static_cast<QwtPlotCurve*>(item);
        if(p)
        {
            return p->pen().color();
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        QwtPlotIntervalCurve* p = static_cast<QwtPlotIntervalCurve*>(item);
        if(p)
        {
            return p->pen().color();
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        QwtPlotHistogram* p = static_cast<QwtPlotHistogram*>(item);
        if(p)
        {
            return p->brush().color();
        }
    }
    default:
        break;
    }
    return QColor();
}
///
/// \brief 获取item的数据个数
/// \param item
/// \return -1 is meaning nan
///
int SAChart::getItemDataSize(QwtPlotItem *item)
{
    switch (item->rtti()) {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        QwtPlotCurve* p = static_cast<QwtPlotCurve*>(item);
        if(p)
        {
            return p->data()->size();
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        QwtPlotIntervalCurve* p = static_cast<QwtPlotIntervalCurve*>(item);
        if(p)
        {
            return p->data()->size();
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        QwtPlotHistogram* p = static_cast<QwtPlotHistogram*>(item);
        if(p)
        {
            return p->data()->size();
        }
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        QwtPlotBarChart* p = static_cast<QwtPlotBarChart*>(item);
        if(p)
        {
            return p->data()->size();
        }
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        QwtPlotMultiBarChart* p = static_cast<QwtPlotMultiBarChart*>(item);
        if(p)
        {
            return p->data()->size();
        }
    }
    default:
        break;
    }
    return -1;
}

void SAChart::setAxisEnable(QwtPlot *chart, int axisID, bool b)
{
    if(chart)
    {
        chart->enableAxis(axisID,b);
        if(!chart->axisAutoScale(axisID))
        {
            chart->setAxisAutoScale(axisID);
        }
    }
}

void SAChart::setAxisTitle(QwtPlot *chart, int axisID, const QString &text)
{
    if(chart)
    {
        chart->setAxisTitle(axisID,text);
    }
}



void SAChart::setAxisFont(QwtPlot*chart,int axisID, const QFont &font)
{
    if(chart)
    {
        chart->setAxisFont(axisID,font);
    }
}

void SAChart::setAxisLabelRotation(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        chart->setAxisLabelRotation(axisID,v);
    }
}

void SAChart::setAxisScaleMin(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        QwtInterval inv = chart->axisInterval(axisID);
        chart->setAxisScale(axisID,v,inv.maxValue());
    }
}

void SAChart::setAxisScaleMax(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        QwtInterval inv = chart->axisInterval(axisID);
        chart->setAxisScale(axisID,inv.minValue(),v);
    }
}

void SAChart::setAxisBorderDistStart(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setBorderDist(v,ax->endBorderDist());
    }
}

void SAChart::setAxisBorderDistEnd(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setBorderDist(ax->startBorderDist(),v);
    }
}

void SAChart::setAxisMargin(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setMargin(v);
    }
}

void SAChart::setAxisSpacing(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setSpacing(v);
    }
}

void SAChart::setAxisLabelAlignment(QwtPlot *chart, int axisID, Qt::Alignment v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setLabelAlignment(v);
    }
}

QwtDateScaleDraw* SAChart::setAxisDateTimeScale(QwtPlot *chart, int axisID, const QString &format, QwtDate::IntervalType type)
{
    if(nullptr == chart)
    {
        return nullptr;
    }
    QwtDateScaleDraw* dateScale;
    dateScale = new QwtDateScaleDraw;//原来的scaleDraw会再qwt自动delete
    dateScale->setDateFormat(type,format);
    chart->setAxisScaleDraw(axisID,dateScale);

    QwtDateScaleEngine* scaleEngine = dynamic_cast<QwtDateScaleEngine*>(chart->axisScaleEngine(axisID));
    if(nullptr == scaleEngine)
    {
        scaleEngine = new QwtDateScaleEngine;
        chart->setAxisScaleEngine(axisID,scaleEngine);
    }
    return dateScale;
}

QwtDateScaleDraw *SAChart::getAxisDateTimeScale(QwtPlot *chart, int axisID)
{
    if(nullptr == chart)
    {
        return nullptr;
    }
    QwtScaleDraw* scale = chart->axisScaleDraw(axisID);
    return dynamic_cast<QwtDateScaleDraw*>(scale);
}

QwtScaleDraw *SAChart::setAxisNormalScale(QwtPlot *chart, int axisID)
{
    if(nullptr == chart)
    {
        return nullptr;
    }
    QwtScaleDraw* scale = nullptr;
    scale = new QwtScaleDraw;

    chart->setAxisScaleDraw(axisID,scale);
    QwtLinearScaleEngine* scaleEngine = dynamic_cast<QwtLinearScaleEngine*>(chart->axisScaleEngine(axisID));
    if(nullptr == scaleEngine)
    {
        scaleEngine = new QwtLinearScaleEngine;
        chart->setAxisScaleEngine(axisID,scaleEngine);
    }
    return scale;
}

///
/// \brief 获取对应坐标轴的id
/// \param axisID
/// \return
///
///
int SAChart::otherAxis(int axisID)
{
    switch(axisID)
    {
    case QwtPlot::xBottom:return QwtPlot::xTop;
    case QwtPlot::xTop:return QwtPlot::xBottom;
    case QwtPlot::yLeft:return QwtPlot::yRight;
    case QwtPlot::yRight:return QwtPlot::yLeft;
    default:return QwtPlot::xBottom;
    }
    return QwtPlot::xBottom;
}
