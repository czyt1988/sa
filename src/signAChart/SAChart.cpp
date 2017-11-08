#include "SAChart.h"
#include "SASeries.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_barchart.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#include "qwt_scale_draw.h"
#include "qwt_date_scale_engine.h"
#include "qwt_date_scale_draw.h"
#include "qwt_plot_multi_barchart.h"
#include <numeric>
///
/// \brief item的类型判断，cureve bar 等绘图相关返回true
/// \param item
/// \return cureve bar 等绘图相关返回true
///
QwtPlotItemList SAChart::getCurveItemList(QwtPlot *chart)
{
    const QwtPlotItemList& items = chart->itemList();
    QwtPlotItemList res;
    for(int i=0;i<items.size();++i)
    {
        if(isPlotCurveItem(items[i]))
        {
            res.append(items[i]);
        }
    }
    return res;
}

bool SAChart::isPlotCurveItem(QwtPlotItem *item)
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
        return true;
    default:
        return false;
    }
    return false;
}
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

///
/// \brief 是否允许显示坐标轴
/// \param chart
/// \param axisID
/// \param b
///
void SAChart::setAxisEnable(QwtPlot *chart, int axisID, bool b)
{
    if(chart)
    {
        chart->enableAxis(axisID,b);
        if(!chart->axisAutoScale(axisID))
        {
            chart->setAxisAutoScale(axisID);
        }
    }
}
///
/// \brief 设置坐标轴的标题
/// \param chart
/// \param axisID
/// \param text
///
void SAChart::setAxisTitle(QwtPlot *chart, int axisID, const QString &text)
{
    if(chart)
    {
        chart->setAxisTitle(axisID,text);
    }
}


///
/// \brief 设置坐标轴文字的字体
/// \param chart
/// \param axisID
/// \param font
///
void SAChart::setAxisFont(QwtPlot*chart,int axisID, const QFont &font)
{
    if(chart)
    {
        chart->setAxisFont(axisID,font);
    }
}
///
/// \brief 设置坐标轴文字的旋转
/// \param chart
/// \param axisID
/// \param v
///
void SAChart::setAxisLabelRotation(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        chart->setAxisLabelRotation(axisID,v);
    }
}
///
/// \brief 设置坐标轴最小刻度
/// \param chart
/// \param axisID
/// \param v
///
void SAChart::setAxisScaleMin(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        QwtInterval inv = chart->axisInterval(axisID);
        chart->setAxisScale(axisID,v,inv.maxValue());
    }
}
///
/// \brief 设置坐标轴最大刻度
/// \param chart
/// \param axisID
/// \param v
///
void SAChart::setAxisScaleMax(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        QwtInterval inv = chart->axisInterval(axisID);
        chart->setAxisScale(axisID,inv.minValue(),v);
    }
}
///
/// \brief 指定坐标轴端点到窗体的距离-影响坐标轴标题的显示
/// \param chart
/// \param axisID
/// \param v
///
void SAChart::setAxisBorderDistStart(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setBorderDist(v,ax->endBorderDist());
    }
}
///
/// \brief 指定坐标轴端点到窗体的距离-影响坐标轴标题的显示
/// \param chart
/// \param axisID
/// \param v
///
void SAChart::setAxisBorderDistEnd(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setBorderDist(ax->startBorderDist(),v);
    }
}
///
/// \brief 设置坐标轴和画板的偏移距离
/// \param chart
/// \param axisID 坐标轴
/// \param v 偏移距离
///
void SAChart::setAxisMargin(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setMargin(v);
    }
}
///
/// \brief 设置坐标轴的间隔
/// \param chart
/// \param axisID 坐标轴
/// \param v 间隔
///
void SAChart::setAxisSpacing(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setSpacing(v);
    }
}
///
/// \brief 设置坐标轴文字的对齐方式
/// \param chart
/// \param axisID 坐标轴
/// \param v 对齐方式
///
void SAChart::setAxisLabelAlignment(QwtPlot *chart, int axisID, Qt::Alignment v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setLabelAlignment(v);
    }
}
///
/// \brief 设置坐标轴为时间坐标
/// \param chart
/// \param axisID 坐标轴id
/// \param format 时间格式
/// \param type 间隔类型
/// \return
///
QwtDateScaleDraw* SAChart::setAxisDateTimeScale(QwtPlot *chart, int axisID, const QString &format, QwtDate::IntervalType type)
{
    if(nullptr == chart)
    {
        return nullptr;
    }
    QwtDateScaleDraw* dateScale;
    dateScale = new QwtDateScaleDraw;//原来的scaleDraw会再qwt自动delete
    dateScale->setDateFormat(type,format);
    chart->setAxisScaleDraw(axisID,dateScale);

    QwtDateScaleEngine* scaleEngine = dynamic_cast<QwtDateScaleEngine*>(chart->axisScaleEngine(axisID));
    if(nullptr == scaleEngine)
    {
        scaleEngine = new QwtDateScaleEngine;
        chart->setAxisScaleEngine(axisID,scaleEngine);
    }
    return dateScale;
}
///
/// \brief 获取时间坐标轴，若当前不是时间坐标轴，返回nullptr,可以用来判断是否为时间坐标轴
/// \param chart
/// \param axisID
/// \return 若当前不是时间坐标轴，返回nullptr
///
QwtDateScaleDraw *SAChart::getAxisDateTimeScale(QwtPlot *chart, int axisID)
{
    if(nullptr == chart)
    {
        return nullptr;
    }
    QwtScaleDraw* scale = chart->axisScaleDraw(axisID);
    return dynamic_cast<QwtDateScaleDraw*>(scale);
}
///
/// \brief 设置为普通线性坐标轴
/// \param chart
/// \param axisID
/// \return
///
QwtScaleDraw *SAChart::setAxisNormalScale(QwtPlot *chart, int axisID)
{
    if(nullptr == chart)
    {
        return nullptr;
    }
    QwtScaleDraw* scale = nullptr;
    scale = new QwtScaleDraw;

    chart->setAxisScaleDraw(axisID,scale);
    QwtLinearScaleEngine* scaleEngine = dynamic_cast<QwtLinearScaleEngine*>(chart->axisScaleEngine(axisID));
    if(nullptr == scaleEngine)
    {
        scaleEngine = new QwtLinearScaleEngine;
        chart->setAxisScaleEngine(axisID,scaleEngine);
    }
    return scale;
}

///
/// \brief 获取对应坐标轴的id
/// 如 xTop会返回xBottom
/// \param axisID
/// \return
///
///
int SAChart::otherAxis(int axisID)
{
    switch(axisID)
    {
    case QwtPlot::xBottom:return QwtPlot::xTop;
    case QwtPlot::xTop:return QwtPlot::xBottom;
    case QwtPlot::yLeft:return QwtPlot::yRight;
    case QwtPlot::yRight:return QwtPlot::yLeft;
    default:return QwtPlot::xBottom;
    }
    return QwtPlot::xBottom;
}

///
/// \brief 获取vector point的y
/// \param xys vector point
/// \param ys vector double y
/// \return 获取的尺寸
///
size_t SAChart::getYDatas(const QVector<QPointF> &xys, QVector<double> &ys)
{
    auto e = xys.cend();
    auto s = 0;
    for(auto i=xys.cbegin();i!=e;++i,++s)
    {
        ys.push_back((*i).y());
    }
    return s;
}
///
/// \brief 获取vector point的x
/// \param xys vector point
/// \param xs vector double x
/// \return 获取的尺寸
///
size_t SAChart::getXDatas(const QVector<QPointF> &xys, QVector<double> &xs)
{
    auto e = xys.cend();
    auto s = 0;
    for(auto i=xys.cbegin();i!=e;++i,++s)
    {
        xs.push_back((*i).x());
    }
    return s;
}
///
/// \brief 获取一个曲线的y值
/// \param ys 结果
/// \param cur 曲线
/// \param rang 指定范围
/// \return
///
size_t SAChart::getYDatas(QVector<double> &ys,const QwtPlotCurve *cur, const QRectF &rang)
{
    const QwtSeriesData<QPointF>* datas = cur->data();
    size_t size = datas->size();
    size_t realSize = 0;
    if(!rang.isNull() && rang.isValid())
    {
        for(size_t i=0;i<size;++i)
        {
            if(rang.contains(datas->sample(i)))
            {
                ys.push_back(datas->sample(i).y());
                ++realSize;
            }
        }
    }
    else
    {
        for(size_t i=0;i<size;++i)
        {
            ys.push_back(datas->sample(i).y());
        }
        realSize = size;
    }
    return realSize;
}
///
/// \brief 获取一个曲线的x值
/// \param xs 结果
/// \param cur 曲线
/// \param rang 指定范围
/// \return
///
size_t SAChart::getXDatas(QVector<double> &xs,const QwtPlotCurve *cur, const QRectF &rang)
{
    const QwtSeriesData<QPointF>* datas = cur->data();
    size_t size = datas->size();
    size_t realSize = 0;
    if(!rang.isNull() && rang.isValid())
    {
        for(size_t i=0;i<size;++i)
        {
            if(rang.contains(datas->sample(i)))
            {
                xs.push_back(datas->sample(i).x());
                ++realSize;
            }
        }
    }
    else
    {
        for(size_t i=0;i<size;++i)
        {
            xs.push_back(datas->sample(i).x());
        }
        realSize = size;
    }
    return realSize;
}
///
/// \brief 获取一个曲线的xy值
/// \param xys
/// \param cur
/// \param rang
/// \return
///
size_t SAChart::getXYDatas(QVector<QPointF> &xys, const QwtPlotCurve *cur, const QRectF &rang)
{
    const QwtSeriesData<QPointF>* datas = cur->data();
    size_t size = datas->size();
    size_t realSize = 0;
    if(!rang.isNull() && rang.isValid())
    {
        for(size_t i=0;i<size;++i)
        {
            if(rang.contains(datas->sample(i)))
            {
                xys.push_back(datas->sample(i));
                ++realSize;
            }
        }
    }
    else
    {
        for(size_t i=0;i<size;++i)
        {
            xys.push_back(datas->sample(i));
        }
        realSize = size;
    }
    return realSize;
}

size_t SAChart::getXYDatas(QVector<double> &xs, QVector<double> &ys, const QwtPlotCurve *cur, const QRectF &rang)
{
    const QwtSeriesData<QPointF>* datas = cur->data();
    size_t size = datas->size();
    size_t realSize = 0;
    if(!rang.isNull() && rang.isValid())
    {
        for(size_t i=0;i<size;++i)
        {
            QPointF p = datas->sample(i);
            if(rang.contains(p))
            {
                ys.push_back(p.y());
                xs.push_back(p.x());
                ++realSize;
            }
        }
    }
    else
    {
        for(size_t i=0;i<size;++i)
        {
            QPointF p = datas->sample(i);
            ys.push_back(p.y());
            xs.push_back(p.x());
        }
        realSize = size;
    }
    return realSize;
}

size_t SAChart::getXYDatas(QVector<QPointF> &xys, QVector<double> &xs, QVector<double> &ys, const QwtPlotCurve *cur, const QRectF &rang)
{
    const QwtSeriesData<QPointF>* datas = cur->data();
    size_t size = datas->size();
    size_t realSize = 0;
    if(!rang.isNull() && rang.isValid())
    {
        for(size_t i=0;i<size;++i)
        {
            QPointF p = datas->sample(i);
            if(rang.contains(p))
            {
                xys.push_back(p);
                ys.push_back(p.y());
                xs.push_back(p.x());
                ++realSize;
            }
        }
    }
    else
    {
        for(size_t i=0;i<size;++i)
        {
            QPointF p = datas->sample(i);
            xys.push_back(p);
            ys.push_back(p.y());
            xs.push_back(p.x());
        }
        realSize = size;
    }
    return realSize;
}
///
/// \brief 设置曲线标识符
/// \param cur 曲线
/// \param style 符号类型
/// \param size 符号尺寸
///
void SAChart::setCurveSymbol(QwtPlotCurve *cur, QwtSymbol::Style style, const QSize &size)
{
    QBrush brush = cur->brush();
    QColor brushColor = brush.color();
    brushColor.setAlpha(80);
    brush.setColor(brushColor);
    QPen pen = cur->pen();
    QwtSymbol* symbol = new QwtSymbol(style,brush,pen,size);
    cur->setSymbol(symbol);
}
///
/// \brief 设置曲线的线形
/// \param cur
/// \param style
///
void SAChart::setCurveLinePenStyle(QwtPlotCurve *cur, Qt::PenStyle style)
{
    QPen pen = cur->pen();
    pen.setStyle(style);
    cur->setPen(pen);
}
///
/// \brief 设置曲线的线形
/// \param cur 曲线
/// \param style
///
void SAChart::setCurvePenStyle(QwtPlotCurve *cur, Qt::PenStyle style)
{
    QPen pen = cur->pen();
    pen.setStyle(style);
    cur->setPen(pen);
}
///
/// \brief 把范围内的数据移除
/// \param removeRang 需要移除的数据范围
/// \param curve 需要移除数据的曲线
/// \return
///
int SAChart::removeDataInRang(const QRectF &removeRang, QwtPlotCurve *curve)
{
    size_t length = curve->data()->size();
    QVector<QPointF> newLine;
    newLine.reserve(length);
    QPointF point;
    for(size_t i = 0;i<length;++i)
    {
        point = curve->data()->sample(i);
        if(removeRang.contains(point))
            continue;
        newLine.push_back(point);
    }
    curve->setSamples(newLine);
    return newLine.size();
}

int SAChart::removeDataInRang(const QPainterPath &removeRang, QwtPlotCurve *curve)
{
    size_t length = curve->data()->size();
    QVector<QPointF> newLine;
    newLine.reserve(length);
    QPointF point;
    for(size_t i = 0;i<length;++i)
    {
        point = curve->data()->sample(i);
        if(removeRang.contains(point))
            continue;
        newLine.push_back(point);
    }
    curve->setSamples(newLine);
    return newLine.size();
}

int SAChart::getDataInRang(const QPainterPath &rang, QwtPlotCurve *curve, QVector<QPointF> &res)
{
    QPointF point;
    size_t length = curve->data()->size();
    for(size_t i = 0;i<length;++i)
    {
        point = curve->data()->sample(i);
        if(rang.contains(point))
        {
            res.push_back(point);
        }
    }
    return res.size();
}
