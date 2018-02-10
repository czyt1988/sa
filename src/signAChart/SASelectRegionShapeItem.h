#ifndef SASELECTREGIONSHAPEITEM_H
#define SASELECTREGIONSHAPEITEM_H
#include "qwt_plot_shapeitem.h"
#include "SAChartGlobals.h"
class SA_CHART_EXPORT SASelectRegionShapeItem : public QwtPlotShapeItem
{
public:
    SASelectRegionShapeItem(const QString &title = QString::null);
    virtual void draw( QPainter *p,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &rect ) const;
    void setEllipse(const QRectF &rect);
};

#endif // SASELECTREGIONSHAPEITEM_H
