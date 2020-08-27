#include "SAIconHelper.h"
#include "qwt_plot_item.h"
#define ICON_FIGURE			QIcon(":/windowIcons/icons/windowIcon/figureWindow.svg")
#define ICON_LINECHART			QIcon(":/icons/icons/lineChart.svg")
#define ICON_BARCHART			QIcon(":/icons/icons/barChart.svg")
#define ICON_BOXCHART			QIcon(":/icons/icons/boxChart.svg")
#define ICON_SCATTERCHART		QIcon(":/icons/icons/scatterChart.svg")
#define ICON_SPECTROCHART		QIcon(":/icons/icons/SpectroChart.svg")
#define ICON_INTERVALCHART		QIcon(":/icons/icons/IntervalChart.svg")
#define ICON_HISTOGRAMCHART		QIcon(":/icons/icons/histogramChart.svg")
#define ICON_MULTIBARCHART		QIcon(":/icons/icons/multiBarChart.svg")
#define ICON_SHAPECHART			QIcon(":/icons/icons/shapeChart.svg")
#define ICON_SVGCHART			QIcon(":/icons/icons/svgChart.svg")
#define ICON_MARKER			QIcon(":/icons/icons/Marker.svg")
#define ICON_LEGEND			QIcon(":/icons/icons/legend.svg")
#define ICON_GRID			QIcon(":/icons/icons/grid.svg")
#define ICON_TEXTLABEL			QIcon(":/icons/icons/TextLabel.svg")
#define ICON_MARKER			QIcon(":/icons/icons/Marker.svg")
#define ICON_ZONE			QIcon(":/icons/icons/zoneItem.svg")
#define ICON_USER_DEFINE		QIcon(":/icons/icons/UserDefineItem.svg")
#define ICON_UNKNOW_ITEM		QIcon(":/icons/icons/UnknowItem.svg")
#define ICON_LAYOUT_VISIBLE		QIcon(":/icons/icons/layerView.svg")
#define ICON_LAYOUT_NOTVISIBLE		QIcon(":/icons/icons/layerNotView.svg")
SAIconHelper::SAIconHelper()
{
}


QIcon SAIconHelper::getIconByPlotItem(const QwtPlotItem *item)
{
    switch (item->rtti())
    {
    case QwtPlotItem::Rtti_PlotItem: //Unspecific value, that can be used, when it doesn't matter
        return (ICON_UNKNOW_ITEM);

    case QwtPlotItem::Rtti_PlotGrid: //For QwtPlotGrid
        return (ICON_GRID);

    case QwtPlotItem::Rtti_PlotScale://For QwtPlotScaleItem
        return (ICON_SCATTERCHART);

    case QwtPlotItem::Rtti_PlotLegend://For QwtPlotLegendItem
        return (ICON_LEGEND);

    case QwtPlotItem::Rtti_PlotMarker://For QwtPlotMarker
        return (ICON_MARKER);

    case QwtPlotItem::Rtti_PlotCurve://For QwtPlotCurve
        return (ICON_LINECHART);

    case QwtPlotItem::Rtti_PlotSpectroCurve://For QwtPlotSpectroCurve
        return (ICON_SPECTROCHART);

    case QwtPlotItem::Rtti_PlotIntervalCurve://For QwtPlotIntervalCurve
        return (ICON_INTERVALCHART);

    case QwtPlotItem::Rtti_PlotHistogram://For QwtPlotHistogram
        return (ICON_HISTOGRAMCHART);

    case QwtPlotItem::Rtti_PlotSpectrogram://For QwtPlotSpectrogram
        return (ICON_SPECTROCHART);

    case QwtPlotItem::Rtti_PlotSVG://For QwtPlotSvgItem
        return (ICON_SVGCHART);

    case QwtPlotItem::Rtti_PlotTradingCurve://For QwtPlotTradingCurve
        return (ICON_BOXCHART);

    case QwtPlotItem::Rtti_PlotBarChart://For QwtPlotBarChart
        return (ICON_BARCHART);

    case QwtPlotItem::Rtti_PlotMultiBarChart://For QwtPlotMultiBarChart
        return (ICON_MULTIBARCHART);

    case QwtPlotItem::Rtti_PlotShape://For QwtPlotShapeItem
        return (ICON_SHAPECHART);

    case QwtPlotItem::Rtti_PlotTextLabel://For QwtPlotTextLabel
        return (ICON_TEXTLABEL);

    case QwtPlotItem::Rtti_PlotZone://For QwtPlotZoneItem
        return (ICON_ZONE);

    case QwtPlotItem::Rtti_PlotUserItem:
        return (ICON_USER_DEFINE);

    default:
        break;
    }
    return (ICON_UNKNOW_ITEM);
}


QIcon SAIconHelper::getFigureWindowIcon()
{
    return (ICON_FIGURE);
}


QIcon SAIconHelper::getLayoutVisibleIcon(bool visible)
{
    if (visible) {
        return (ICON_LAYOUT_VISIBLE);
    }
    return (ICON_LAYOUT_NOTVISIBLE);
}
