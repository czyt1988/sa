#include "SAChartSerializeHelp.h"
#include "SAQwtSerialize.h"
#include "qwt_plot_curve.h"
#include "SAChart.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#include "SAXYSeries.h"
#include "SAAbstractDatas.h"
#include "qwt_plot_barchart.h"
#include "SABarSeries.h"
#include "SAScatterSeries.h"
#define VERSION_LEN (32)
using namespace sa;
///
/// \brief SAChart2D的序列化文件头
///
struct chart2d_serialize_header
{
    int magicStart;
    QString externInfo;
    chart2d_serialize_header()
        :magicStart(0xfd3548)
    {

    }
    bool isValid() const
    {
        return (magicStart == 0xfd3548);
    }
};

///
/// \brief item的序列化文件头
///
struct chart_item_rtti_info
{
    int magicStart;
    int rtti;///< item的rtti
    bool isEnd;///< 是否是结束，如果是，将会结束
    int magicEnd;
    chart_item_rtti_info()
        :magicStart(0xabcdf12)
        ,magicEnd(0x24645f)
        ,rtti(0)
        ,isEnd(true)
    {

    }
    bool isValid() const
    {
        return ((magicStart == 0xabcdf12) && (magicEnd == 0x24645f));
    }
};

QDataStream& operator <<(QDataStream & out,const chart2d_serialize_header& header);
QDataStream& operator >>(QDataStream & in,chart2d_serialize_header& header);

QDataStream& operator <<(QDataStream & out,const chart_item_rtti_info& header);
QDataStream& operator >>(QDataStream & in,chart_item_rtti_info& header);

QDataStream& operator <<(QDataStream & out,const chart2d_serialize_header& header)
{
    out << header.magicStart
        << header.externInfo
           ;
    return out;
}

QDataStream& operator >>(QDataStream & in, chart2d_serialize_header &header)
{
    in >> header.magicStart
       >> header.externInfo
            ;
    return in;
}

QDataStream& operator <<(QDataStream & out,const chart_item_rtti_info& header)
{
    out << header.magicStart
        << header.rtti
        << header.isEnd
        << header.magicEnd
           ;
    return out;
}

QDataStream& operator >>(QDataStream & in,chart_item_rtti_info& header)
{
    in >> header.magicStart
       >> header.rtti
       >> header.isEnd
       >> header.magicEnd
            ;
    return in;
}


SAChartSerializeHelp::SAChartSerializeHelp()
{

}


void SAChartSerializeHelp::serializeOut(QDataStream &out, const SAChart2D *chart)
{
    chart2d_serialize_header header;
    const unsigned int check0 = 0xd1a23d;
    const unsigned int check1 = 0xd123d;
    out << header;
    out << check0;
    out << static_cast<const QwtPlot *>(chart);
    out << check1;
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
    const unsigned int check0 = 0xd1a23d;
    const unsigned int check1 = 0xd123d;
    unsigned int tmp;
    in >> header;
    if(!header.isValid())
    {
        throw sa::SABadSerializeExpection();
        return;
    }

    in >> tmp;
    if(check0 != tmp)
    {
        throw sa::SABadSerializeExpection();
        return;
    }
    in >> static_cast<QwtPlot *>(chart);
    in >> tmp;
    if(check1 != tmp)
    {
        throw sa::SABadSerializeExpection();
        return;
    }
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
    const unsigned int check0 = 0x8754af;
    const unsigned int check1 = 0xff323a;
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
            out << header << check0 << p << check1;
            break;
        }
        case QwtPlotItem::Rtti_PlotBarChart:
        {
            QwtPlotBarChart* p = static_cast<QwtPlotBarChart*>(item);
            out << header << check0 << p << check1;
            break;
        }
        case QwtPlotItem::Rtti_PlotSpectroCurve:
        case QwtPlotItem::Rtti_PlotIntervalCurve:
        case QwtPlotItem::Rtti_PlotHistogram:
        case QwtPlotItem::Rtti_PlotSpectrogram:
        case QwtPlotItem::Rtti_PlotTradingCurve:
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
    if(!header.isValid())
    {
        throw sa::SABadSerializeExpection();
        return;
    }
    const unsigned int check0 = 0x8754af;
    const unsigned int check1 = 0xff323a;
    unsigned int tmp0,tmp1;
    while(!header.isEnd)
    {
        switch(header.rtti)
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {
            QwtPlotCurve* p = new QwtPlotCurve;
            in >> tmp0 >> p >> tmp1;
            if(check0 != tmp0 || check1 != tmp1)
            {
                delete p;
                throw sa::SABadSerializeExpection();
                return;
            }
            p->attach(chart);
            break;
        }
        case QwtPlotItem::Rtti_PlotBarChart:
        {
            QwtPlotBarChart* p = new SABarSeries;
            in >> tmp0 >> p >> tmp1;
            if(check0 != tmp0 || check1 != tmp1)
            {
                delete p;
                throw sa::SABadSerializeExpection();
                return;
            }
            p->attach(chart);
            break;
        }
        default:
            break;
        }
        in >> header;
        if(!header.isValid())
        {
            throw sa::SABadSerializeExpection();
            return;
        }
    }
    return;
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
    unsigned int c0=0x67fa34;
    out << c0;
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
    unsigned int c0=0x67fa34;
    unsigned int tmpCheck;
    in >> tmpCheck;
    if(c0 != tmpCheck)
    {
        throw sa::SABadSerializeExpection();
        return in;
    }
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
    const SASeriesAndDataPtrMapper *m = static_cast<const SASeriesAndDataPtrMapper*>(p);
    out << c
        << m;
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
    SASeriesAndDataPtrMapper *m = static_cast<SASeriesAndDataPtrMapper*>(p);
    in >> c
       >> m;
    return in;
}




///
/// \brief SABarSeries的序列化
/// \param p
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const SABarSeries *p)
{
    const QwtPlotBarChart* c = static_cast<const QwtPlotBarChart*>(p);
    const SASeriesAndDataPtrMapper *m = static_cast<const SASeriesAndDataPtrMapper*>(p);
    out << c << m;
    return out;
}
///
/// \brief SABarSeries的序列化
/// \param in
/// \param p
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, SABarSeries *p)
{
    QwtPlotBarChart* c = static_cast<QwtPlotBarChart*>(p);
    SASeriesAndDataPtrMapper *m = static_cast<SASeriesAndDataPtrMapper*>(p);
    in >> c >> m;
    return in;
}
///
/// \brief SAScatterSeries的序列化
/// \param out
/// \param p
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const SAScatterSeries *p)
{
    const QwtPlotCurve* c = static_cast<const QwtPlotCurve*>(p);
    const SASeriesAndDataPtrMapper *m = static_cast<const SASeriesAndDataPtrMapper*>(p);
    out << c << m;
    return out;
}
///
/// \brief SAScatterSeries的序列化
/// \param in
/// \param p
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, SAScatterSeries *p)
{
    QwtPlotCurve* c = static_cast<QwtPlotCurve*>(p);
    SASeriesAndDataPtrMapper *m = static_cast<SASeriesAndDataPtrMapper*>(p);
    in >> c >> m;
    return in;
}
