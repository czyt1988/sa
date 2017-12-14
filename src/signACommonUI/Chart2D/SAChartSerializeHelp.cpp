#include "SAChartSerializeHelp.h"
#include "SAQwtSerialize.h"
#include "qwt_plot_curve.h"
#include "SAChart.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#include "SAXYSeries.h"
#include "SAAbstractDatas.h"
#define VERSION_LEN (32)
using namespace sa;
///
/// \brief SAChart2D的序列化文件头
///
struct chart2d_serialize_header
{
    QString externInfo;
};

///
/// \brief item的序列化文件头
///
struct chart_item_rtti_info
{
    int rtti;///< item的rtti
    bool isEnd;///< 是否是结束，如果是，将会结束
};

QDataStream& operator <<(QDataStream & out,const chart2d_serialize_header& header);
QDataStream& operator >>(QDataStream & in,chart2d_serialize_header& header);

QDataStream& operator <<(QDataStream & out,const chart_item_rtti_info& header);
QDataStream& operator >>(QDataStream & in,chart_item_rtti_info& header);

QDataStream& operator <<(QDataStream & out,const chart2d_serialize_header& header)
{
    out << header.externInfo
           ;
    return out;
}

QDataStream& operator >>(QDataStream & in, chart2d_serialize_header &header)
{
    in >> header.externInfo
            ;
    return in;
}

QDataStream& operator <<(QDataStream & out,const chart_item_rtti_info& header)
{
    out << header.rtti
        << header.isEnd
           ;
    return out;
}

QDataStream& operator >>(QDataStream & in,chart_item_rtti_info& header)
{
    in >> header.rtti
       >> header.isEnd
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
        break;
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
        break;
    }
}

void SAChartSerializeHelp::serializeOut(QDataStream &out, const SAChart2D *chart)
{
    chart2d_serialize_header header;
    out << header;
    out << static_cast<const QwtPlot *>(chart);
    out << chart->isEnableGrid()
        << chart->isEnableGridX()
        << chart->isEnableGridY()
        << chart->isEnableGridXMin()
        << chart->isEnableGridYMin()
        << chart->isEnableLegend()
        << chart->isEnableLegendPanel()
           ;
    //序列化曲线的数据
    serializeOutChartItem(out,chart);
}

void SAChartSerializeHelp::serializeIn(QDataStream &in, SAChart2D *chart)
{
    chart2d_serialize_header header;
    in >> header;
    in >> static_cast<QwtPlot *>(chart);
    bool isEnableGrid,isEnableGridX,isEnableGridY
            ,isEnableGridXMin,isEnableGridYMin
            ,isEnableLegend,isEnableLegendPanel;
    in  >> isEnableGrid
        >> isEnableGridX
        >> isEnableGridY
        >> isEnableGridXMin
        >> isEnableGridYMin
        >> isEnableLegend
        >> isEnableLegendPanel
            ;
    chart->enableGrid(isEnableGrid);
    chart->enableGridX(isEnableGridX);
    chart->enableGridX(isEnableGridY);
    chart->enableGridX(isEnableGridXMin);
    chart->enableGridX(isEnableGridYMin);
    chart->enableGridX(isEnableLegend);
    chart->enableGridX(isEnableLegendPanel);
    //序列化曲线的数据
    serializeInChartItem(in,chart);
}

void SAChartSerializeHelp::serializeOutChartItem(QDataStream &out, const SAChart2D *chart)
{
    const QList<QwtPlotItem*> items = chart->itemList();
    const int count = items.size();
    for(int i=0 ;i<count;++i)
    {
        QwtPlotItem* item = items[i];
        chart_item_rtti_info header;
        header.rtti = item->rtti();
        header.isEnd = false;
        switch(item->rtti())
        {
        case QwtPlotItem::Rtti_PlotCurve://序列化曲线
        {

            QwtPlotCurve* p = static_cast<QwtPlotCurve*>(item);
            out << header << p;
            break;
        }
        case SA::RTTI_SAXYSeries:
        {
            SAXYSeries* p = static_cast<SAXYSeries*>(item);
            out << header << p;
            break;
        }
        case QwtPlotItem::Rtti_PlotSpectroCurve:
        case QwtPlotItem::Rtti_PlotIntervalCurve:
        case QwtPlotItem::Rtti_PlotHistogram:
        case QwtPlotItem::Rtti_PlotSpectrogram:
        case QwtPlotItem::Rtti_PlotTradingCurve:
        case QwtPlotItem::Rtti_PlotBarChart:
        case QwtPlotItem::Rtti_PlotMultiBarChart:
            break;
        default:
            break;
        }
    }
    //item结束标记
    chart_item_rtti_info header;
    header.rtti = 0;
    header.isEnd = true;
    out << header;
}

void SAChartSerializeHelp::serializeInChartItem(QDataStream &in, SAChart2D *chart)
{
    chart_item_rtti_info header;
    in >> header;
    while(!header.isEnd)
    {
        switch(header.rtti)
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {
            QwtPlotCurve* p = new QwtPlotCurve;
            in >> p;
            p->attach(chart);
            break;
        }
        case SA::RTTI_SAXYSeries:
        {
            SAXYSeries * p = new SAXYSeries;
            in >> p;
            p->attach(chart);
            break;
        }
        default:
            break;
        }
        in >> header;
    }
}

///
/// \brief SASeriesAndDataPtrMapper的序列化
/// \param out
/// \param p
/// \return
///
QDataStream & sa::operator <<(QDataStream &out, const SASeriesAndDataPtrMapper *p)
{
    const QSet<SAAbstractDatas*>& linkDatas = p->linkDatas();
    //保存变量名
    out << linkDatas.size();
    for(auto i=linkDatas.begin();i!=linkDatas.end();++i)
    {
        out << (*i)->getName();
    }
    return out;
}
///
/// \brief SASeriesAndDataPtrMapper的序列化
/// \param in
/// \param p
/// \return
///
QDataStream & sa::operator >>(QDataStream &in, SASeriesAndDataPtrMapper *p)
{
    int count = 0;
    in >> count;
    QStringList valList;
    QString tmp;
    for(int i=0;i<count;++i)
    {
        in >> tmp;
        valList.append(tmp);
    }
//TODO:

    return in;
}
///
/// \brief SAXYSeries的序列化
/// \param out
/// \param p
/// \return
///
QDataStream & sa::operator <<(QDataStream &out, const SAXYSeries *p)
{
    const QwtPlotCurve* c = static_cast<const QwtPlotCurve*>(p);
    //const SASeriesAndDataPtrMapper *m = static_cast<const SASeriesAndDataPtrMapper*>(p);
    out << c;
    return out;
}
///
/// \brief SAXYSeries的序列化
/// \param in
/// \param p
/// \return
///
QDataStream & sa::operator >>(QDataStream &in, SAXYSeries *p)
{
    QwtPlotCurve* c = static_cast<QwtPlotCurve*>(p);
    //SASeriesAndDataPtrMapper *m = static_cast<SASeriesAndDataPtrMapper*>(p);
    in >> c ;
    return in;
}




