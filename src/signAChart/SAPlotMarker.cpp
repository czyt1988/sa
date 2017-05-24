#include "SAPlotMarker.h"

SAAbstractPlotMarker::SAAbstractPlotMarker(const QString &title):QwtPlotMarker(title)
{

}

SAAbstractPlotMarker::SAAbstractPlotMarker(const QwtText &title):QwtPlotMarker(title)
{

}

SAAbstractPlotMarker::~SAAbstractPlotMarker()
{

}



SAPointMarker::SAPointMarker(const QString &title):SAAbstractPlotMarker(title)
{

}

SAPointMarker::SAPointMarker(const QwtText &title):SAAbstractPlotMarker(title)
{

}

SAPointMarker::SAPointMarker(const QPointF &point)
    :SAAbstractPlotMarker(QString("PointMarker:(%1,%2)")
                          .arg(point.x())
                          .arg(point.y()))
{
    setRenderHint( QwtPlotItem::RenderAntialiased, true );
    setValue(point);
}

SAPointMarker::~SAPointMarker()
{

}



SAYValueMarker::SAYValueMarker(const QString &title):SAAbstractPlotMarker(title)
{

}

SAYValueMarker::SAYValueMarker(const QwtText &title):SAAbstractPlotMarker(title)
{

}

SAYValueMarker::SAYValueMarker(const double &value):SAAbstractPlotMarker(
                                                        QString("YValueMarker:%1")
                                                              .arg(value)
                                                        )
{
    QPen pen(QColor(Qt::black),1);
    setLineStyle(QwtPlotMarker::HLine);
    setYValue(value);
    setLinePen(pen);
}

SAYValueMarker::~SAYValueMarker()
{

}
