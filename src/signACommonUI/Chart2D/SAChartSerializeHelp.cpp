#include "SAChartSerializeHelp.h"
#include "SAQwtSerialize.h"
#include "qwt_plot_curve.h"
#include "SAChart.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#define VERSION_LEN (32)
using namespace sa;
struct SAChart2DSerializeHeader
{
    QString externInfo;
};

QDataStream& operator <<(QDataStream & out,const SAChart2DSerializeHeader& header);
QDataStream& operator >>(QDataStream & in,SAChart2DSerializeHeader& header);

QDataStream& operator <<(QDataStream & out,const SAChart2DSerializeHeader& header)
{
    out << header.externInfo
           ;
    return out;
}

QDataStream& operator >>(QDataStream & in, SAChart2DSerializeHeader &header)
{
    in >> header.externInfo
            ;
    return in;
}

SAChartSerializeHelp::SAChartSerializeHelp()
{

}
///
/// \brief 序列化QwtPlotItem
/// \param out
/// \param item
///
void SAChartSerializeHelp::serializeOut(QDataStream &out, const QwtPlotItem *item)
{
    switch(item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        const QwtPlotCurve* c = static_cast<const QwtPlotCurve*>(item);
        out << c;
        break;
    }
    default:
        out << item;
    }
}

void SAChartSerializeHelp::serializeIn(QDataStream &in, QwtPlotItem *item)
{
    switch(item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        QwtPlotCurve* c = static_cast<QwtPlotCurve*>(item);
        in >> c;
        break;
    }
    default:
        in >> item;
    }
}

void SAChartSerializeHelp::serializeOut(QDataStream &out, const SAChart2D *chart)
{
    SAChart2DSerializeHeader header;
    out << header;
    out << static_cast<const QwtPlot *>(chart);
}

void SAChartSerializeHelp::serializeIn(QDataStream &in, SAChart2D *chart)
{
    SAChart2DSerializeHeader header;
    in >> header;
    in >> static_cast<QwtPlot *>(chart);
}




