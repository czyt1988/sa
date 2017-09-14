#ifndef SASELECTREGIONSHAPEITEM_H
#define SASELECTREGIONSHAPEITEM_H
#include "qwt_plot_shapeitem.h"
#include "SAChartGlobals.h"
class SA_CHART_EXPORT SASelectRegionShapeItem : public QwtPlotShapeItem
{
public:
    SASelectRegionShapeItem(const QString &title = QString::null);
    virtual int rtti() const;
};

#endif // SASELECTREGIONSHAPEITEM_H
