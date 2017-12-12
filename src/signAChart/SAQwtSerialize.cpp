#include "SAQwtSerialize.h"
#include "SAChart.h"
#include "qwt_plot_item.h"
#include "qwt_plot_curve.h"
#include "qwt_plot.h"
#include "qwt_symbol.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#include "qwt_color_map.h"
namespace sa {
    void serialize_out_scale_widge(QDataStream &out, const QwtPlot *chart,int axis);
    void serialize_in_scale_widge(QDataStream &in, QwtPlot *chart,int axis);
}


void sa::serialize_out_scale_widge(QDataStream &out, const QwtPlot *chart,int axis)
{
    const QwtScaleWidget* axisWid = chart->axisWidget(axis);
    bool isaxis = (nullptr != axisWid);
    out << isaxis;
    if(isaxis)
    {
        out << axisWid;
    }
}

void sa::serialize_in_scale_widge(QDataStream &in, QwtPlot *chart, int axis)
{
    bool isaxis;
    in >> isaxis;
    if(isaxis)
    {
        QwtScaleWidget* axisWid = chart->axisWidget(axis);
        if(nullptr == axisWid)
        {
            chart->enableAxis(axis);
            axisWid = chart->axisWidget(axis);
        }
        if(nullptr == axisWid)
        {
            QwtScaleWidget w;
            QwtScaleWidget* pw = &w;
            in>>pw;
            return;
        }
        in>>axisWid;
    }
}

QDataStream &sa::operator <<(QDataStream &out, const QwtText &t)
{
    out << t.text()
        << t.font()
        << t.renderFlags()
        << t.color()
        << t.borderRadius()
        << t.borderPen()
        << t.backgroundBrush()
        << t.testPaintAttribute(QwtText::PaintUsingTextFont)
        << t.testPaintAttribute(QwtText::PaintUsingTextColor)
        << t.testPaintAttribute(QwtText::PaintBackground)
        << t.testLayoutAttribute(QwtText::MinimumLayout)
           ;
    return out;
}

QDataStream &sa::operator >>(QDataStream &in, QwtText &t)
{
    QString str;
    QFont font;
    int renderFlags;
    QColor clr;
    double borderRadius;
    QPen borderPen;
    QBrush backgroundBrush;
    bool isPaintUsingTextFont,isPaintUsingTextColor,isPaintBackground;
    bool isMinimumLayout;
    in  >> str
        >> font
        >> renderFlags
        >> clr
        >> borderRadius
        >> borderPen
        >> backgroundBrush
        >> isPaintUsingTextFont
        >> isPaintUsingTextColor
        >> isPaintBackground
        >> isMinimumLayout
            ;
    t.setText(str);
    t.setFont(font);
    t.setRenderFlags(renderFlags);
    t.setColor(clr);
    t.setBorderRadius(borderRadius);
    t.setBorderPen(borderPen);
    t.setBackgroundBrush(backgroundBrush);
    t.setPaintAttribute(QwtText::PaintUsingTextFont,isPaintUsingTextFont);
    t.setPaintAttribute(QwtText::PaintUsingTextColor,isPaintUsingTextColor);
    t.setPaintAttribute(QwtText::PaintBackground,isPaintBackground);
    t.setLayoutAttribute(QwtText::MinimumLayout,isMinimumLayout);
    return in;
}
///
/// \brief QwtSymbol 指针的序列化
/// \param out
/// \param t
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const QwtSymbol *t)
{
    out << static_cast<int>(t->cachePolicy())
        << t->size()
        << t->pinPoint()
        << t->isPinPointEnabled()
        << t->brush()
        << t->pen()
        << static_cast<int>(t->style())
        << t->path()
        << t->pixmap()
           ;
    return out;
}
///
/// \brief QwtSymbol 指针的序列化
/// \param in
/// \param t
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, QwtSymbol *t)
{
    int cachePolicy;
    QSize size;
    QPointF pinPoint;
    bool isPinPointEnabled;
    QBrush brush;
    QPen pen;
    int style;
    QPainterPath path;
    QPixmap pixmap;
    in >> cachePolicy
            >> size
            >> pinPoint
            >> isPinPointEnabled
            >> brush
            >> pen
            >> style
            >> path
            >> pixmap
            ;
    t->setCachePolicy(static_cast<QwtSymbol::CachePolicy>(cachePolicy));
    t->setSize(size);
    t->setPinPoint(pinPoint);
    t->setPinPointEnabled(isPinPointEnabled);
    t->setBrush(brush);
    t->setPen(pen);
    t->setStyle(static_cast<QwtSymbol::Style>(style));
    t->setPath(path);
    t->setPixmap(pixmap);
    return in;
}
///
/// \brief QwtPlotItem序列化
/// \param out
/// \param item
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const QwtPlotItem *item)
{
    out << item->title()
        << item->z()
        << item->isVisible()
        << item->xAxis()
        << item->yAxis()
        << item->legendIconSize()
        << item->testItemAttribute(QwtPlotItem::Legend)
        << item->testItemAttribute(QwtPlotItem::AutoScale)
        << item->testItemAttribute(QwtPlotItem::Margins)
        << item->testItemInterest(QwtPlotItem::ScaleInterest)
        << item->testItemInterest(QwtPlotItem::LegendInterest)
        << item->testRenderHint(QwtPlotItem::RenderAntialiased)
        << item->renderThreadCount()
           ;
    return out;
}
///
/// \brief QwtPlotItem序列化
/// \param in
/// \param item
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, QwtPlotItem *item)
{
    QwtText title;
    double z;
    bool isVisible;
    int xaxis,yaxis;
    QSize legendIconSize;
    bool isLegend,isAutoScale,isMargins;
    bool isScaleInterest,isLegendInterest;
    bool isRenderAntialiased;
    uint renderThreadCount;
    in >> title
            >> z
            >> isVisible
            >> xaxis
            >> yaxis
            >> legendIconSize
            >> isLegend >> isAutoScale >> isMargins
            >> isScaleInterest >> isLegendInterest
            >> isRenderAntialiased
            >> renderThreadCount
            ;
    item->setTitle(title);
    item->setZ(z);
    item->setVisible(isVisible);
    item->setAxes(xaxis,yaxis);
    item->setLegendIconSize(legendIconSize);
    item->setItemAttribute(QwtPlotItem::Legend,isLegend);
    item->setItemAttribute(QwtPlotItem::AutoScale,isAutoScale);
    item->setItemAttribute(QwtPlotItem::Margins,isMargins);
    item->setItemInterest(QwtPlotItem::ScaleInterest,isScaleInterest);
    item->setItemInterest(QwtPlotItem::LegendInterest,isLegendInterest);
    item->setRenderHint(QwtPlotItem::RenderAntialiased,isRenderAntialiased);
    item->setRenderThreadCount(renderThreadCount);
    return in;
}


///
/// \brief QwtPlotCurve指针的序列化
/// \param out
/// \param item
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const QwtPlotCurve *item)
{
    out << (const QwtPlotItem*)item;
    out << item->baseline()
        << item->brush()
        << item->pen()
        << static_cast<int>(item->style())
        << item->testPaintAttribute(QwtPlotCurve::ClipPolygons)
        << item->testPaintAttribute(QwtPlotCurve::FilterPoints)
        << item->testPaintAttribute(QwtPlotCurve::MinimizeMemory)
        << item->testPaintAttribute(QwtPlotCurve::ImageBuffer)
        << item->testLegendAttribute(QwtPlotCurve::LegendNoAttribute)
        << item->testLegendAttribute(QwtPlotCurve::LegendShowLine)
        << item->testLegendAttribute(QwtPlotCurve::LegendShowSymbol)
        << item->testLegendAttribute(QwtPlotCurve::LegendShowBrush)
        << item->testCurveAttribute(QwtPlotCurve::Inverted)
        << item->testCurveAttribute(QwtPlotCurve::Fitted)
           ;
    //QwtSymbol的序列化
    const QwtSymbol* symbol = item->symbol();
    bool isHaveSymbol = (symbol != nullptr);
    out << isHaveSymbol;
    if(isHaveSymbol)
    {
        out << symbol;
    }
    //save sample
    QVector<QPointF> sample;
    SAChart::getXYDatas(sample,item);
    out << sample;
    return out;
}
///
/// \brief QwtPlotCurve指针的序列化
/// \param in
/// \param item
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, QwtPlotCurve *item)
{
    double baseline;
    QBrush brush;
    QPen pen;
    int style;
    bool isClipPolygons;
    bool isFilterPoints;
    bool isMinimizeMemory;
    bool isImageBuffer;
    bool isLegendNoAttribute;
    bool isLegendShowLine;
    bool isLegendShowSymbol;
    bool isLegendShowBrush;
    bool isInverted;
    bool isFitted;
    in >> (QwtPlotItem*)item;
    in >> baseline
            >> brush
            >> pen
            >> style
            >> isClipPolygons
            >> isFilterPoints
            >> isMinimizeMemory
            >> isImageBuffer
            >> isLegendNoAttribute
            >> isLegendShowLine
            >> isLegendShowSymbol
            >> isLegendShowBrush
            >> isInverted
            >> isFitted
            ;
    item->setBaseline(baseline);
    item->setBrush(brush);
    item->setPen(pen);
    item->setStyle(static_cast<QwtPlotCurve::CurveStyle>(style));
    item->setPaintAttribute(QwtPlotCurve::ClipPolygons,isClipPolygons);
    item->setPaintAttribute(QwtPlotCurve::FilterPoints,isFilterPoints);
    item->setPaintAttribute(QwtPlotCurve::MinimizeMemory,isMinimizeMemory);
    item->setPaintAttribute(QwtPlotCurve::ImageBuffer,isImageBuffer);
    item->setLegendAttribute(QwtPlotCurve::LegendNoAttribute,isLegendNoAttribute);
    item->setLegendAttribute(QwtPlotCurve::LegendShowLine,isLegendShowLine);
    item->setLegendAttribute(QwtPlotCurve::LegendShowSymbol,isLegendShowSymbol);
    item->setLegendAttribute(QwtPlotCurve::LegendShowBrush,isLegendShowBrush);
    item->setCurveAttribute(QwtPlotCurve::Inverted,isInverted);
    item->setCurveAttribute(QwtPlotCurve::Fitted,isFitted);
    // QwtSymbol的序列化
    bool isHaveSymbol;
    in >> isHaveSymbol;
    if(isHaveSymbol)
    {
        QwtSymbol* symbol = new QwtSymbol();
        in >> symbol;
        item->setSymbol(symbol);
    }
    //load sample
    QVector<QPointF> sample;
    in >> sample;
    item->setSamples(sample);
    return in;
}


///
/// \brief QwtScaleWidget指针的序列化
/// \param out
/// \param w
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const QwtScaleWidget *w)
{
    int minBorderDistStart,minBorderDistEnd;
    w->getMinBorderDist(minBorderDistStart,minBorderDistEnd);
    out << w->title()
        << w->testLayoutFlag(QwtScaleWidget::TitleInverted)
        << w->startBorderDist()
        << w->endBorderDist()
        << minBorderDistStart
        << minBorderDistEnd
        << w->margin()
        << w->spacing()
        << w->isColorBarEnabled()
        << w->colorBarWidth()
        << static_cast<int>(w->alignment())
        << w->colorBarInterval()
           ;
    //QwtScaleDraw
    const QwtScaleDraw* sd = w->scaleDraw();
    bool isSD = (sd != nullptr);
    if(isSD)
    {
        out << isSD
            << sd;
    }
    return out;
}

///
/// \brief QwtScaleWidget指针的序列化
/// \param in
/// \param w
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, QwtScaleWidget *w)
{
    QwtText title;
    bool isTitleInverted;
    int startBorderDist,endBorderDist;
    int minBorderDistStart,minBorderDistEnd;
    int margin,spacing;
    bool isColorBarEnabled;
    int colorBarWidth;
    int alignment;
    QwtInterval colorBarInterval;
    in >> title
            >> isTitleInverted
            >> startBorderDist
            >> endBorderDist
            >> minBorderDistStart
            >> minBorderDistEnd
            >> margin
            >> spacing
            >> isColorBarEnabled
            >> colorBarWidth
            >> alignment
            >> colorBarInterval
            ;
    w->setTitle(title);
    w->setLayoutFlag(QwtScaleWidget::TitleInverted,isTitleInverted);
    w->setBorderDist(startBorderDist,endBorderDist);
    w->setMinBorderDist(minBorderDistStart,minBorderDistEnd);
    w->setMargin(margin);
    w->setSpacing(spacing);
    w->setColorBarEnabled(isColorBarEnabled);
    w->setColorBarWidth(colorBarWidth);
    w->setAlignment(static_cast<QwtScaleDraw::Alignment>(alignment));
    //QwtScaleDraw
    bool isSD;
    in >> isSD;
    if(isSD)
    {
        QwtScaleDraw* sd = w->scaleDraw();
        if(nullptr == sd)
        {
            w->setScaleDraw(new QwtScaleDraw());
            sd = w->scaleDraw();
        }
        in >> sd;
    }
    return in;
}
///
/// \brief  QwtScaleDraw指针的序列化
/// \param out
/// \param w
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const QwtScaleDraw *w)
{
    out << static_cast<int>(w->alignment())
        << w->length()
        << static_cast<int>(w->labelAlignment())
        << w->labelRotation()
           ;
    return out;
}
///
/// \brief  QwtScaleDraw指针的序列化
/// \param in
/// \param w
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, QwtScaleDraw *w)
{
    int alignment;
    double length;
    int labelAlignment;
    double labelRotation;
    in >> alignment
            >> length
            >> labelAlignment
            >> labelRotation
            ;
    w->setAlignment(static_cast<QwtScaleDraw::Alignment>(alignment));
    w->setLength(length);
    w->setLabelAlignment(static_cast<Qt::Alignment>(labelAlignment));
    w->setLabelRotation(labelRotation);

    return in;
}
///
/// \brief QFrame的序列化
/// \param out
/// \param f
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const QFrame *f)
{
    out << static_cast<int>(f->frameShadow())
        << static_cast<int>(f->frameShape())
        << f->frameStyle()
        << f->lineWidth()
        << f->midLineWidth()
        << f->palette()
        << f->styleSheet()
           ;
    return out;
}
///
/// \brief QFrame的序列化
/// \param in
/// \param f
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, QFrame *f)
{
    int frameShadow;
    int frameShape;
    int frameStyle;
    int lineWidth;
    int midLineWidth;
    QPalette pl;
    QString styleSheet;
    in >> frameShadow
            >> frameShape
            >> frameStyle
            >> lineWidth
            >> midLineWidth
            >> pl
            >> styleSheet
            ;
    f->setFrameShadow(static_cast<QFrame::Shadow>(frameShadow));
    f->setFrameShape(static_cast<QFrame::Shape>(frameShadow));
    f->setFrameStyle(frameStyle);
    f->setLineWidth(lineWidth);
    f->setMidLineWidth(midLineWidth);
    f->setPalette(pl);
    if(!styleSheet.isEmpty())
    {
        f->setStyleSheet(styleSheet);
    }
    return in;
}

///
/// \brief QwtPlotCanvas的序列化
/// \param out
/// \param c
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const QwtPlotCanvas *c)
{
    out << (const QFrame*)c;
    out << static_cast<int>(c->focusIndicator())
        << c->borderRadius()
        << c->testPaintAttribute(QwtPlotCanvas::BackingStore)
        << c->testPaintAttribute(QwtPlotCanvas::Opaque)
        << c->testPaintAttribute(QwtPlotCanvas::HackStyledBackground)
        << c->testPaintAttribute(QwtPlotCanvas::ImmediatePaint)
           ;
    return out;
}
///
/// \brief QwtPlotCanvas的序列化
/// \param in
/// \param c
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, QwtPlotCanvas *c)
{
    in >> (QFrame*)c;
    int focusIndicator;
    double borderRadius;
    bool isBackingStore,isOpaque,isHackStyledBackground,isImmediatePaint;
    in >> focusIndicator
            >> borderRadius
            >> isBackingStore
            >> isOpaque
            >> isHackStyledBackground
            >> isImmediatePaint
            ;
    c->setFocusIndicator(static_cast<QwtPlotCanvas::FocusIndicator>(focusIndicator));
    c->setBorderRadius(borderRadius);
    c->setPaintAttribute(QwtPlotCanvas::BackingStore,isBackingStore);
    c->setPaintAttribute(QwtPlotCanvas::Opaque,isOpaque);
    c->setPaintAttribute(QwtPlotCanvas::HackStyledBackground,isHackStyledBackground);
    c->setPaintAttribute(QwtPlotCanvas::ImmediatePaint,isImmediatePaint);
    return in;
}



///
/// \brief SAChart2D的序列化
/// \param out
/// \param chart
/// \return
///
QDataStream &sa::operator <<(QDataStream &out, const QwtPlot *chart)
{
    //QFrame save
    out << (const QFrame*)chart;
    //QwtPlot save
    out << chart->title()
        << chart->footer()
        << chart->palette()
           ;
    //axis
    serialize_out_scale_widge(out,chart,QwtPlot::xTop);
    serialize_out_scale_widge(out,chart,QwtPlot::xBottom);
    serialize_out_scale_widge(out,chart,QwtPlot::yLeft);
    serialize_out_scale_widge(out,chart,QwtPlot::yRight);
    //QwtPlotCanvas save
    const QwtPlotCanvas* canvas = qobject_cast<const QwtPlotCanvas*>(chart->canvas());
    bool isDefaultCanvas = (canvas != nullptr);
    out << isDefaultCanvas;
    if(isDefaultCanvas)
    {
        out << canvas;
    }
    return out;
}
///
/// \brief SAChart2D的序列化
/// \param in
/// \param chart
/// \return
///
QDataStream &sa::operator >>(QDataStream &in, QwtPlot *chart)
{
    //QFrame load
    in >> (QFrame*)chart;
    //QwtPlot load
    QwtText title;
    QwtText footer;
    QPalette pl;
    in >> title
       >> footer
       >>pl
            ;
    chart->setTitle(title);
    chart->setFooter(footer);
    chart->setPalette(pl);
    //axis
    serialize_in_scale_widge(in,chart,QwtPlot::xTop);
    serialize_in_scale_widge(in,chart,QwtPlot::xBottom);
    serialize_in_scale_widge(in,chart,QwtPlot::yLeft);
    serialize_in_scale_widge(in,chart,QwtPlot::yRight);
    //QwtPlotCanvas load
    bool isDefaultCanvas;
    in >> isDefaultCanvas;
    if(isDefaultCanvas)
    {
        QwtPlotCanvas* canvas = qobject_cast<QwtPlotCanvas*>(chart->canvas());
        if(nullptr == canvas)
        {
            canvas = new QwtPlotCanvas(chart);
            chart->setCanvas(canvas);
        }
        in >> canvas;
    }
    return in;
}

///
/// \brief QwtIntervalSample序列化支持
/// \param out
/// \param item
/// \return
///
QDataStream &sa::operator<<(QDataStream & out, const QwtIntervalSample & item)
{
    out << item.value << item.interval;
    return out;
}
///
/// \brief QwtIntervalSample序列化支持
/// \param out
/// \param item
/// \return
///
QDataStream &sa::operator>>(QDataStream & in, QwtIntervalSample & item)
{
    in >> item.value >> item.interval;
    return in;
}
///
/// \brief QwtInterval序列化支持
/// \param out
/// \param item
/// \return
///
QDataStream &sa::operator<<(QDataStream & out, const QwtInterval & item)
{
    out << item.minValue() << item.maxValue() << item.borderFlags();
    return out;
}
///
/// \brief QwtInterval序列化支持
/// \param out
/// \param item
/// \return
///
QDataStream &sa::operator>>(QDataStream & in, QwtInterval & item)
{
    int flag;
    double min,max;
    in >> min >> max >> flag;
    item.setMinValue(min);
    item.setMaxValue(max);
    item.setBorderFlags(static_cast<QwtInterval::BorderFlags>(flag));
    return in;
}



///
/// \brief QwtColorMap序列化
/// \param out
/// \param c
/// \return
///
//QDataStream &sa::operator <<(QDataStream &out, const QwtColorMap *c)
//{

//}
///
/// \brief QwtColorMap序列化
/// \param in
/// \param c
/// \return
///
//QDataStream &sa::operator >>(QDataStream &in, QwtColorMap *c)
//{

//}


