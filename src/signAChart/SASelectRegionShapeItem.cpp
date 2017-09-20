#include "SASelectRegionShapeItem.h"
#include "SAChart.h"
#include <QPen>
#include <QBrush>
SASelectRegionShapeItem::SASelectRegionShapeItem(const QString &title)
    :QwtPlotShapeItem(title)
{
    setItemAttribute( QwtPlotItem::Legend, false );
    setRenderHint( QwtPlotItem::RenderAntialiased, true );
    QColor fillColor(Qt::blue);
    fillColor.setAlpha( 10 );
    QPen pen( Qt::black, 1 );
    pen.setStyle(Qt::DashDotLine);
    pen.setJoinStyle( Qt::MiterJoin );
    setPen( pen );
    setBrush( fillColor );
}

int SASelectRegionShapeItem::rtti() const
{
    return SAChart::RTTI_SA_SELECT_REGION_SHAPE;
}

void SASelectRegionShapeItem::draw(QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &rect) const
{
    QwtPlotShapeItem::draw(p,xMap,yMap,rect);
}
