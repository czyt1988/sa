#include "SACrossTracker.h"
#include "qwt_picker_machine.h"
#include "qwt_plot.h"
#include "qwt_date_scale_draw.h"

SACrossTracker::SACrossTracker(QWidget *w)
    :QwtPlotPicker(w)
{
    init();
}

SACrossTracker::SACrossTracker(int xAxis, int yAxis, QWidget *w)
   :QwtPlotPicker(xAxis,yAxis,w)
{
    init();
}



void SACrossTracker::init()
{
    setTrackerMode( QwtPlotPicker::AlwaysOn );//这是指定文字的显示，AlwaysOn值，光标不激活，也显示文字提示
    setRubberBand( QwtPlotPicker::CrossRubberBand );
    setStateMachine(  new QwtPickerTrackerMachine() );//QwtPickerTrackerMachine是不用鼠标激活
    setRubberBandPen(  QPen( QColor(186,85,211) ) );
}

QwtText SACrossTracker::trackerTextF(const QPointF &pos) const
{
    QString s("");
    const QwtScaleDraw* sd = plot()->axisScaleDraw(QwtPlot::xBottom);
    if(sd != nullptr)
    {
        const QwtDateScaleDraw* dsd = dynamic_cast<const QwtDateScaleDraw*>(sd);
        if(dsd != nullptr)
        {
            //说明坐标轴是时间轴
            s += QStringLiteral("(%1,").arg(dsd->label(pos.x()).text());
        }
        else
        {
            s += QString("(%1,").arg(pos.x());
        }
    }
    sd = plot()->axisScaleDraw(QwtPlot::yLeft);
    if(sd != nullptr)
    {
        const QwtDateScaleDraw* dsd = dynamic_cast<const QwtDateScaleDraw*>(sd);
        if(dsd != nullptr)
        {
            //说明坐标轴是时间轴
            s += QStringLiteral("%1)").arg(dsd->label(pos.y()).text());
        }
        else
        {
            s += QString("%2)").arg(pos.y());
        }
    }
    //axisScaleDraw


    QwtText text( s );
    text.setColor( Qt::white );
    QColor c = rubberBandPen().color();
    text.setBorderPen( QPen( c ) );
    text.setBorderRadius( 6 );
    c.setAlpha( 200 );
    text.setBackgroundBrush( c );

    return text;
}
