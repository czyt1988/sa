#include "SAYDataTracker.h"
#include <qwt_plot_item.h>
#include <qwt_plot_curve.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_dict.h>
#include <qwt_plot_barchart.h>
#include <qwt_plot.h>
#include <qwt_column_symbol.h>
#include <numeric>
#include <QPalette>
SAYDataTracker::SAYDataTracker( QWidget *canvas ):
QwtPlotPicker( canvas )
{
    setTrackerMode( QwtPlotPicker::ActiveOnly );
    setRubberBand( VLineRubberBand );

    setStateMachine( new QwtPickerTrackerMachine() );
}

QRect SAYDataTracker::trackerRect( const QFont &font ) const
{
    QRect r = QwtPlotPicker::trackerRect( font );
    r.moveTop(pickArea().boundingRect().top());
    return r;
}

QwtText SAYDataTracker::trackerTextF( const QPointF &pos ) const
{
    QwtText trackerText;

    trackerText.setColor( Qt::black );

    QColor c(200,200,200,100);
    trackerText.setBorderPen( QPen( c, 2 ) );
    trackerText.setBackgroundBrush( c );

    QString info;
#if 0
    const QwtPlotItemList curves =
        plot()->itemList( QwtPlotItem::Rtti_PlotCurve );

    for ( int i = 0; i < curves.size(); i++ )
    {
        const QString curveInfo = curveInfoAt(
            static_cast<const QwtPlotCurve *>( curves[i] ), pos );

        if ( !curveInfo.isEmpty() )
        {
            if ( !info.isEmpty() )
                info += "<br>";

            info += curveInfo;
        }
    }
#else
    const QwtPlotItemList& items = plot()->itemList();
    for ( int i = 0; i < items.size(); i++ )
    {
        QString curveInfo;
        switch(items[i]->rtti())
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {
            curveInfo = curveInfoAt(static_cast<const QwtPlotCurve *>( items[i] ), pos );
            break;
        }
        case QwtPlotItem::Rtti_PlotBarChart:
        {
            curveInfo = barInfoAt(static_cast<const QwtPlotBarChart *>( items[i] ), pos );
            break;
        }
        default:
            break;
        }
        if( !curveInfo.isEmpty() )
        {
            if ( !info.isEmpty() )
                info += "<br>";
            info += curveInfo;
        }
    }
#endif
    trackerText.setText( info );
    return trackerText;
}

QString SAYDataTracker::curveInfoAt(const QwtPlotCurve *curve, const QPointF &pos ) const
{
    const QLineF line = curveLineAt( curve, pos.x() );
    if ( line.isNull() )
        return QString::null;

    const double y = line.pointAt(
        ( pos.x() - line.p1().x() ) / line.dx() ).y();

    QString info( "<font color=""%1"">%2</font>" );
    return info.arg( curve->pen().color().name() ).arg( y );
}

QLineF SAYDataTracker::curveLineAt(const QwtPlotCurve *curve, double x ) const
{
    QLineF line;

    if ( curve->dataSize() >= 2 )
    {
        const QRectF br = curve->boundingRect();
        if ( br.isValid() && x >= br.left() && x <= br.right() )
        {
            int index = qwtUpperSampleIndex<QPointF>(
                            *curve->data(), x
                            ,[](const double x, const QPointF &pos)->bool{
                                return ( x < pos.x() );
                            } );

            if ( index == -1 &&
                x == curve->sample( curve->dataSize() - 1 ).x() )
            {
                // the last sample is excluded from qwtUpperSampleIndex
                index = curve->dataSize() - 1;
            }

            if ( index > 0 )
            {
                line.setP1( curve->sample( index - 1 ) );
                line.setP2( curve->sample( index ) );
            }
        }
    }

    return line;
}

QString SAYDataTracker::barInfoAt(const QwtPlotBarChart *bar, const QPointF &pos) const
{
    const double y = barValueAt( bar, pos.x() );
    if(y == std::numeric_limits<double>::min())
    {
        return QString::null;
    }
    QColor clr = bar->symbol()->palette().color(QPalette::Button);
    QString info( "<font color=""%1"">%2</font>" );
    return info.arg( clr.name() ).arg( y );
}

double SAYDataTracker::barValueAt(const QwtPlotBarChart *bar, double x) const
{
    if ( bar->dataSize() >= 2 )
    {
        const QRectF br = bar->boundingRect();
        if ( br.isValid() && x >= br.left() && x <= br.right() )
        {
            int index = qwtUpperSampleIndex<QPointF>(
                            *bar->data(), x
                            ,[](const double& x1, const QPointF &p)->bool{
                                return ( x1 < p.x() );
                            });
            if ( index == -1 &&
                x == bar->sample( bar->dataSize() - 1 ).x() )
            {
                // the last sample is excluded from qwtUpperSampleIndex
                index = bar->dataSize() - 1;
            }
            if ( index > 0 )
            {
                return bar->sample(index).y();
            }
        }
    }
    return std::numeric_limits<double>::min();
}



