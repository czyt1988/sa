#include "SAChart.h"
#include "SASeries.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_barchart.h"
#include "qwt_plot_multi_barchart.h"
#include <numeric>
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
