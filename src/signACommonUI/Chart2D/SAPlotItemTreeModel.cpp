#include "SAPlotItemTreeModel.h"
#include "SAChart.h"

SAPlotItemTreeModel::SAPlotItemTreeModel(QObject * parent):QwtPlotItemTreeModel(parent)
{

}

QList<QwtPlotItem *> SAPlotItemTreeModel::getItems(QwtPlot *plot) const
{
#ifdef SA_COMMON_UI_USE_DYNAMIC
    return SAChart::dynamicGetPlotChartItemList(plot);
#else
    const QwtPlotItemList& items = chart->itemList();
    QwtPlotItemList res;
    for(int i=0;i<items.size();++i)
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
        case SA::RTTI_SAXYSeries:
        case SA::RTTI_SABarSeries:
        case SA::RTTI_SABoxSeries:
        case SA::RTTI_SAHistogramSeries:
        case SA::RTTI_SAScatterSeries:
            res.append(items[i]);
            break;
        default:
            break;
        }
    }
    return res;
#endif
}
