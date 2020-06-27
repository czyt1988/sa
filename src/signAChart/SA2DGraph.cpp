#include "SA2DGraph.h"
#include "SAChart.h"
#include <qstyle.h>
#include <qstyleoption.h>

#include "qwt_interval.h"
#include "qwt_picker_machine.h"
#include <algorithm>
#include "qwt_legend_label.h"
#include "qwt_date_scale_draw.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_barchart.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_multi_barchart.h"
#include "qwt_plot_tradingcurve.h"
#include "qwt_plot_spectrocurve.h"
#include "qwt_plot_spectrogram.h"
#include "SAPlotMarker.h"
#include "SAYDataTracker.h"
#include "SAXYDataTracker.h"
#include "SACrossTracker.h"
//unsigned int ChartWave_qwt::staticValue_nAutoLineID = 0;//静态变量初始化

class SA2DGraphPrivate
{
    SA_IMPL_PUBLIC(SA2DGraph)
public:
    QScopedPointer<SAPlotZoomer> m_zoomer;
    QScopedPointer<SAPlotZoomer> m_zoomerSecond;
    QwtPlotGrid *m_grid;
    QwtPlotPicker *m_picker;
    QwtPlotPanner* m_panner;
    LegendItem* m_legend;
    QwtLegend* m_legendPanel;
    SAYDataTracker* m_yDataPicker;
    SAXYDataTracker* m_xyDataPicker;
    SA2DGraphPrivate(SA2DGraph* p):q_ptr(p)
      ,m_grid(nullptr)
      ,m_picker(nullptr)
      ,m_panner(nullptr)
      ,m_legend(nullptr)
      ,m_legendPanel(nullptr)
      ,m_yDataPicker(nullptr)
      ,m_xyDataPicker(nullptr)
    {

    }
};

class ScrollData
{
public:
    ScrollData():
        scrollBar( NULL ),
        position( ScrollZoomer::OppositeToScale ),
        mode( Qt::ScrollBarAsNeeded )
    {
    }

    ~ScrollData()
    {
        delete scrollBar;
    }

    ScrollBar *scrollBar;
    ScrollZoomer::ScrollBarPosition position;
    Qt::ScrollBarPolicy mode;
};


ScrollBar::ScrollBar( QWidget * parent ):
    QScrollBar( parent )
{
    init();
}

ScrollBar::ScrollBar( Qt::Orientation o,
        QWidget *parent ):
    QScrollBar( o, parent )
{
    init();
}

ScrollBar::ScrollBar( double minBase, double maxBase,
        Qt::Orientation o, QWidget *parent ):
    QScrollBar( o, parent )
{
    init();
    setBase( minBase, maxBase );
    moveSlider( minBase, maxBase );
}

void ScrollBar::init()
{
    d_inverted = orientation() == Qt::Vertical;
    d_baseTicks = 1000000;
    d_minBase = 0.0;
    d_maxBase = 1.0;
    moveSlider( d_minBase, d_maxBase );

    connect( this, SIGNAL( sliderMoved( int ) ), SLOT( catchSliderMoved( int ) ) );
    connect( this, SIGNAL( valueChanged( int ) ), SLOT( catchValueChanged( int ) ) );
}

void ScrollBar::setInverted( bool inverted )
{
    if ( d_inverted != inverted )
    {
        d_inverted = inverted;
        moveSlider( minSliderValue(), maxSliderValue() );
    }
}

bool ScrollBar::isInverted() const
{
    return d_inverted;
}

void ScrollBar::setBase( double min, double max )
{
    if ( min != d_minBase || max != d_maxBase )
    {
        d_minBase = min;
        d_maxBase = max;

        moveSlider( minSliderValue(), maxSliderValue() );
    }
}

void ScrollBar::moveSlider( double min, double max )
{
    const int sliderTicks = qRound( ( max - min ) /
        ( d_maxBase - d_minBase ) * d_baseTicks );

    // setRange initiates a valueChanged of the scrollbars
    // in some situations. So we block
    // and unblock the signals.

    blockSignals( true );

    setRange( sliderTicks / 2, d_baseTicks - sliderTicks / 2 );
    int steps = sliderTicks / 200;
    if ( steps <= 0 )
        steps = 1;

    setSingleStep( steps );
    setPageStep( sliderTicks );

    int tick = mapToTick( min + ( max - min ) / 2 );
    if ( isInverted() )
        tick = d_baseTicks - tick;

    setSliderPosition( tick );
    blockSignals( false );
}

double ScrollBar::minBaseValue() const
{
    return d_minBase;
}

double ScrollBar::maxBaseValue() const
{
    return d_maxBase;
}

void ScrollBar::sliderRange( int value, double &min, double &max ) const
{
    if ( isInverted() )
        value = d_baseTicks - value;

    const int visibleTicks = pageStep();

    min = mapFromTick( value - visibleTicks / 2 );
    max = mapFromTick( value + visibleTicks / 2 );
}

double ScrollBar::minSliderValue() const
{
    double min, dummy;
    sliderRange( value(), min, dummy );

    return min;
}

double ScrollBar::maxSliderValue() const
{
    double max, dummy;
    sliderRange( value(), dummy, max );

    return max;
}

int ScrollBar::mapToTick( double v ) const
{
    const double pos = ( v - d_minBase ) / ( d_maxBase - d_minBase ) * d_baseTicks;
    return static_cast<int>( pos );
}

double ScrollBar::mapFromTick( int tick ) const
{
    return d_minBase + ( d_maxBase - d_minBase ) * tick / d_baseTicks;
}

void ScrollBar::catchValueChanged( int value )
{
    double min, max;
    sliderRange( value, min, max );
    Q_EMIT valueChanged( orientation(), min, max );
}

void ScrollBar::catchSliderMoved( int value )
{
    double min, max;
    sliderRange( value, min, max );
    Q_EMIT sliderMoved( orientation(), min, max );
}

int ScrollBar::extent() const
{
    QStyleOptionSlider opt;
    opt.init( this );
    opt.subControls = QStyle::SC_None;
    opt.activeSubControls = QStyle::SC_None;
    opt.orientation = orientation();
    opt.minimum = minimum();
    opt.maximum = maximum();
    opt.sliderPosition = sliderPosition();
    opt.sliderValue = value();
    opt.singleStep = singleStep();
    opt.pageStep = pageStep();
    opt.upsideDown = invertedAppearance();
    if ( orientation() == Qt::Horizontal )
        opt.state |= QStyle::State_Horizontal;
    return style()->pixelMetric( QStyle::PM_ScrollBarExtent, &opt, this );
}

ScrollZoomer::ScrollZoomer(int xAxis, int yAxis, QWidget* canvas):QwtPlotZoomer(xAxis,yAxis, canvas )
  ,d_cornerWidget( NULL )
  ,d_hScrollData( NULL )
  ,d_vScrollData( NULL )
  ,d_inZoom( false )
  ,d_isEnable(true)
{
    for ( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
        d_alignCanvasToScales[ axis ] = false;

    if ( !canvas )
        return;

    d_hScrollData = new ScrollData;
    d_vScrollData = new ScrollData;
}

ScrollZoomer::ScrollZoomer( QWidget *canvas ):
    QwtPlotZoomer( canvas ),
    d_cornerWidget( NULL ),
    d_hScrollData( NULL ),
    d_vScrollData( NULL ),
    d_inZoom( false ),
    d_isEnable(true)
{
    for ( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
        d_alignCanvasToScales[ axis ] = false;

    if ( !canvas )
        return;

    d_hScrollData = new ScrollData;
    d_vScrollData = new ScrollData;
}

ScrollZoomer::~ScrollZoomer()
{
    delete d_cornerWidget;
    delete d_vScrollData;
    delete d_hScrollData;
}

void ScrollZoomer::rescale()
{
    QwtScaleWidget *xScale = plot()->axisWidget( xAxis() );
    QwtScaleWidget *yScale = plot()->axisWidget( yAxis() );

    if ( zoomRectIndex() <= 0 )
    {
        if ( d_inZoom )
        {
            xScale->setMinBorderDist( 0, 0 );
            yScale->setMinBorderDist( 0, 0 );

            QwtPlotLayout *layout = plot()->plotLayout();

            for ( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
                layout->setAlignCanvasToScale( axis, d_alignCanvasToScales );

            d_inZoom = false;
        }
    }
    else
    {
        if ( !d_inZoom )
        {
            /*
             We set a minimum border distance.
             Otherwise the canvas size changes when scrolling,
             between situations where the major ticks are at
             the canvas borders (requiring extra space for the label)
             and situations where all labels can be painted below/top
             or left/right of the canvas.
             */
            int start, end;

            xScale->getBorderDistHint( start, end );
            xScale->setMinBorderDist( start, end );

            yScale->getBorderDistHint( start, end );
            yScale->setMinBorderDist( start, end );

            QwtPlotLayout *layout = plot()->plotLayout();
            for ( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
            {
                d_alignCanvasToScales[axis] =
                    layout->alignCanvasToScale( axis );
            }

            layout->setAlignCanvasToScales( false );

            d_inZoom = true;
        }
    }

    QwtPlotZoomer::rescale();
    updateScrollBars();
}

bool ScrollZoomer::isEnableScrollBar() const
{
    return d_isEnable;
}

void ScrollZoomer::on_enable_scrollBar(bool enable)
{
    d_isEnable = enable;
    updateScrollBars();
}

ScrollBar *ScrollZoomer::scrollBar( Qt::Orientation orientation )
{
    ScrollBar *&sb = ( orientation == Qt::Vertical )
        ? d_vScrollData->scrollBar : d_hScrollData->scrollBar;

    if ( sb == NULL )
    {
        sb = new ScrollBar( orientation, canvas() );
        sb->hide();
        connect( sb,
            SIGNAL( valueChanged( Qt::Orientation, double, double ) ),
            SLOT( scrollBarMoved( Qt::Orientation, double, double ) ) );
    }
    return sb;
}

ScrollBar *ScrollZoomer::horizontalScrollBar() const
{
    return d_hScrollData->scrollBar;
}

ScrollBar *ScrollZoomer::verticalScrollBar() const
{
    return d_vScrollData->scrollBar;
}

void ScrollZoomer::setHScrollBarMode( Qt::ScrollBarPolicy mode )
{
    if ( hScrollBarMode() != mode )
    {
        d_hScrollData->mode = mode;
        updateScrollBars();
    }
}

void ScrollZoomer::setVScrollBarMode( Qt::ScrollBarPolicy mode )
{
    if ( vScrollBarMode() != mode )
    {
        d_vScrollData->mode = mode;
        updateScrollBars();
    }
}

Qt::ScrollBarPolicy ScrollZoomer::hScrollBarMode() const
{
    return d_hScrollData->mode;
}

Qt::ScrollBarPolicy ScrollZoomer::vScrollBarMode() const
{
    return d_vScrollData->mode;
}

void ScrollZoomer::setHScrollBarPosition( ScrollBarPosition pos )
{
    if ( d_hScrollData->position != pos )
    {
        d_hScrollData->position = pos;
        updateScrollBars();
    }
}

void ScrollZoomer::setVScrollBarPosition( ScrollBarPosition pos )
{
    if ( d_vScrollData->position != pos )
    {
        d_vScrollData->position = pos;
        updateScrollBars();
    }
}

ScrollZoomer::ScrollBarPosition ScrollZoomer::hScrollBarPosition() const
{
    return d_hScrollData->position;
}

ScrollZoomer::ScrollBarPosition ScrollZoomer::vScrollBarPosition() const
{
    return d_vScrollData->position;
}

void ScrollZoomer::setCornerWidget( QWidget *w )
{
    if ( w != d_cornerWidget )
    {
        if ( canvas() )
        {
            delete d_cornerWidget;
            d_cornerWidget = w;
            if ( d_cornerWidget->parent() != canvas() )
                d_cornerWidget->setParent( canvas() );

            updateScrollBars();
        }
    }
}

QWidget *ScrollZoomer::cornerWidget() const
{
    return d_cornerWidget;
}

bool ScrollZoomer::eventFilter( QObject *object, QEvent *event )
{
    if ( object == canvas() )
    {
        switch( event->type() )
        {
            case QEvent::Resize:
            {
                int left, top, right, bottom;
                canvas()->getContentsMargins( &left, &top, &right, &bottom );

                QRect rect;
                rect.setSize( static_cast<QResizeEvent *>( event )->size() );
                rect.adjust( left, top, -right, -bottom );

                layoutScrollBars( rect );
                break;
            }
            case QEvent::ChildRemoved:
            {
                const QObject *child =
                    static_cast<QChildEvent *>( event )->child();

                if ( child == d_cornerWidget )
                    d_cornerWidget = NULL;
                else if ( child == d_hScrollData->scrollBar )
                    d_hScrollData->scrollBar = NULL;
                else if ( child == d_vScrollData->scrollBar )
                    d_vScrollData->scrollBar = NULL;
                break;
            }
            default:
                break;
        }
    }
    return QwtPlotZoomer::eventFilter( object, event );
}

bool ScrollZoomer::needScrollBar( Qt::Orientation orientation ) const
{
    Qt::ScrollBarPolicy mode;
    double zoomMin, zoomMax, baseMin, baseMax;

    if ( orientation == Qt::Horizontal )
    {
        mode = d_hScrollData->mode;
        baseMin = zoomBase().left();
        baseMax = zoomBase().right();
        zoomMin = zoomRect().left();
        zoomMax = zoomRect().right();
    }
    else
    {
        mode = d_vScrollData->mode;
        baseMin = zoomBase().top();
        baseMax = zoomBase().bottom();
        zoomMin = zoomRect().top();
        zoomMax = zoomRect().bottom();
    }

    bool needed = false;
    switch( mode )
    {
        case Qt::ScrollBarAlwaysOn:
            needed = true;
            break;
        case Qt::ScrollBarAlwaysOff:
            needed = false;
            break;
        default:
        {
            if ( baseMin < zoomMin || baseMax > zoomMax )
                needed = true;
            break;
        }
    }
    return needed;
}

void ScrollZoomer::updateScrollBars()
{
    if ( !canvas() )
        return;

    const int xAxis = QwtPlotZoomer::xAxis();
    const int yAxis = QwtPlotZoomer::yAxis();

    int xScrollBarAxis = xAxis;
    if ( hScrollBarPosition() == OppositeToScale )
        xScrollBarAxis = oppositeAxis( xScrollBarAxis );

    int yScrollBarAxis = yAxis;
    if ( vScrollBarPosition() == OppositeToScale )
        yScrollBarAxis = oppositeAxis( yScrollBarAxis );


    QwtPlotLayout *layout = plot()->plotLayout();

    bool showHScrollBar = needScrollBar( Qt::Horizontal );
    if ( showHScrollBar )
    {
        ScrollBar *sb = scrollBar( Qt::Horizontal );
        sb->setPalette( plot()->palette() );
        sb->setInverted( !plot()->axisScaleDiv( xAxis ).isIncreasing() );
        sb->setBase( zoomBase().left(), zoomBase().right() );
        sb->moveSlider( zoomRect().left(), zoomRect().right() );

        if ( !sb->isVisibleTo( canvas() ) )
        {
            if(d_isEnable)
                sb->show();
            else
                sb->hide();
            layout->setCanvasMargin( layout->canvasMargin( xScrollBarAxis )
                + sb->extent(), xScrollBarAxis );
        }
    }
    else
    {
        if ( horizontalScrollBar() )
        {
            horizontalScrollBar()->hide();
            layout->setCanvasMargin( layout->canvasMargin( xScrollBarAxis )
                - horizontalScrollBar()->extent(), xScrollBarAxis );
        }
    }

    bool showVScrollBar = needScrollBar( Qt::Vertical );
    if ( showVScrollBar )
    {
        ScrollBar *sb = scrollBar( Qt::Vertical );
        sb->setPalette( plot()->palette() );
        sb->setInverted( plot()->axisScaleDiv( yAxis ).isIncreasing() );
        //如果sb->setInverted(! plot()->axisScaleDiv( yAxis ).isIncreasing() );那么向下拉滑动杆，视图向上滚
        sb->setBase( zoomBase().top(), zoomBase().bottom() );
        sb->moveSlider( zoomRect().top(), zoomRect().bottom() );

        if ( !sb->isVisibleTo( canvas() ) )
        {
            if(d_isEnable)
                sb->show();
            else
                sb->hide();
            layout->setCanvasMargin( layout->canvasMargin( yScrollBarAxis )
                + sb->extent(), yScrollBarAxis );
        }
    }
    else
    {
        if ( verticalScrollBar() )
        {
            verticalScrollBar()->hide();
            layout->setCanvasMargin( layout->canvasMargin( yScrollBarAxis )
                - verticalScrollBar()->extent(), yScrollBarAxis );
        }
    }

    if ( showHScrollBar && showVScrollBar )
    {
        if(d_isEnable)
        {
            if ( d_cornerWidget == NULL )
            {
                d_cornerWidget = new QWidget( canvas() );
                d_cornerWidget->setAutoFillBackground( true );
                d_cornerWidget->setPalette( plot()->palette() );
            }
            d_cornerWidget->show();
        }
        else
        {
            if ( d_cornerWidget )
                d_cornerWidget->hide();
        }
    }
    else
    {
        if ( d_cornerWidget )
            d_cornerWidget->hide();
    }

    layoutScrollBars( canvas()->contentsRect() );
    plot()->updateLayout();
}

void ScrollZoomer::layoutScrollBars( const QRect &rect )
{
    int hPos = xAxis();
    if ( hScrollBarPosition() == OppositeToScale )
        hPos = oppositeAxis( hPos );

    int vPos = yAxis();
    if ( vScrollBarPosition() == OppositeToScale )
        vPos = oppositeAxis( vPos );

    ScrollBar *hScrollBar = horizontalScrollBar();
    ScrollBar *vScrollBar = verticalScrollBar();

    const int hdim = hScrollBar ? hScrollBar->extent() : 0;
    const int vdim = vScrollBar ? vScrollBar->extent() : 0;

    if ( hScrollBar && hScrollBar->isVisible() )
    {
        int x = rect.x();
        int y = ( hPos == QwtPlot::xTop )
            ? rect.top() : rect.bottom() - hdim + 1;
        int w = rect.width();

        if ( vScrollBar && vScrollBar->isVisible() )
        {
            if ( vPos == QwtPlot::yLeft )
                x += vdim;
            w -= vdim;
        }

        hScrollBar->setGeometry( x, y, w, hdim );
    }
    if ( vScrollBar && vScrollBar->isVisible() )
    {
        int pos = yAxis();
        if ( vScrollBarPosition() == OppositeToScale )
            pos = oppositeAxis( pos );

        int x = ( vPos == QwtPlot::yLeft )
            ? rect.left() : rect.right() - vdim + 1;
        int y = rect.y();

        int h = rect.height();

        if ( hScrollBar && hScrollBar->isVisible() )
        {
            if ( hPos == QwtPlot::xTop )
                y += hdim;

            h -= hdim;
        }

        vScrollBar->setGeometry( x, y, vdim, h );
    }
    if ( hScrollBar && hScrollBar->isVisible() &&
        vScrollBar && vScrollBar->isVisible() )
    {
        if ( d_cornerWidget )
        {
            QRect cornerRect(
                vScrollBar->pos().x(), hScrollBar->pos().y(),
                vdim, hdim );
            d_cornerWidget->setGeometry( cornerRect );
        }
    }
}

void ScrollZoomer::scrollBarMoved(
    Qt::Orientation o, double min, double max )
{
    Q_UNUSED( max );

    if ( o == Qt::Horizontal )
        moveTo( QPointF( min, zoomRect().top() ) );
    else
        moveTo( QPointF( zoomRect().left(), min ) );

    Q_EMIT zoomed( zoomRect() );
}

int ScrollZoomer::oppositeAxis( int axis ) const
{
    switch( axis )
    {
        case QwtPlot::xBottom:
            return QwtPlot::xTop;
        case QwtPlot::xTop:
            return QwtPlot::xBottom;
        case QwtPlot::yLeft:
            return QwtPlot::yRight;
        case QwtPlot::yRight:
            return QwtPlot::yLeft;
        default:
            break;
    }

    return axis;
}


struct compareX
{
	inline bool operator()( const double x, const QPointF &pos ) const
	{
		return ( x < pos.x() );
	}
};

//SAYDataTracker::SAYDataTracker( QWidget *canvas ):
//QwtPlotPicker( canvas )
//{
//	setTrackerMode( QwtPlotPicker::ActiveOnly );
//	setRubberBand( VLineRubberBand );

//	setStateMachine( new QwtPickerDragPointMachine() );
//}

//QRect SAYDataTracker::trackerRect( const QFont &font ) const
//{
//	QRect r = QwtPlotPicker::trackerRect( font );

//	// align r to the first curve
	
//// 	const QwtPlotItemList curves = plot()->itemList( QwtPlotItem::Rtti_PlotCurve );
//// 	if ( curves.size() > 0 )
//// 	{
//// 		QPointF pos = invTransform( trackerPosition() );
////
//// 		const QLineF line = curveLineAt(
//// 			static_cast<const QwtPlotCurve *>( curves[0] ), pos.x() );
//// 		if ( !line.isNull() )
//// 		{
//// 			const double curveY = line.pointAt(
//// 				( pos.x() - line.p1().x() ) / line.dx() ).y();
////
//// 			pos.setY( curveY );
//// 			pos = transform( pos );
////
//// 			r.moveBottom( pos.y() );
//// 		}
//// 	}

////	int heigth = r.height();
//	r.moveTop(pickArea().boundingRect().top());
//	return r;
//}

//QwtText SAYDataTracker::trackerTextF( const QPointF &pos ) const
//{
//	QwtText trackerText;

//	trackerText.setColor( Qt::black );

//	QColor c(200,200,200,100);
//	trackerText.setBorderPen( QPen( c, 2 ) );
//	trackerText.setBackgroundBrush( c );

//	QString info;

//	const QwtPlotItemList curves =
//		plot()->itemList( QwtPlotItem::Rtti_PlotCurve );

//	for ( int i = 0; i < curves.size(); i++ )
//	{
//		const QString curveInfo = curveInfoAt(
//			static_cast<const QwtPlotCurve *>( curves[i] ), pos );

//		if ( !curveInfo.isEmpty() )
//		{
//			if ( !info.isEmpty() )
//				info += "<br>";

//			info += curveInfo;
//		}
//	}

//	trackerText.setText( info );
//	return trackerText;
//}

//QString SAYDataTracker::curveInfoAt(
//	const QwtPlotCurve *curve, const QPointF &pos ) const
//{
//	const QLineF line = curveLineAt( curve, pos.x() );
//	if ( line.isNull() )
//		return QString::null;

//	const double y = line.pointAt(
//		( pos.x() - line.p1().x() ) / line.dx() ).y();
	
//	QString info( "<font color=""%1"">%2</font>" );
//	return info.arg( curve->pen().color().name() ).arg( y );
//}

//QLineF SAYDataTracker::curveLineAt(
//	const QwtPlotCurve *curve, double x ) const
//{
//	QLineF line;

//	if ( curve->dataSize() >= 2 )
//	{
//		const QRectF br = curve->boundingRect();
//		if ( br.isValid() && x >= br.left() && x <= br.right() )
//		{
//			int index = qwtUpperSampleIndex<QPointF>(
//				*curve->data(), x, compareX() );

//			if ( index == -1 &&
//				x == curve->sample( curve->dataSize() - 1 ).x() )
//			{
//				// the last sample is excluded from qwtUpperSampleIndex
//				index = curve->dataSize() - 1;
//			}

//			if ( index > 0 )
//			{
//				line.setP1( curve->sample( index - 1 ) );
//				line.setP2( curve->sample( index ) );
//			}
//		}
//	}

//	return line;
//}

SA2DGraph::SA2DGraph(QWidget *parent):QwtPlot(parent)
  ,d_ptr(new SA2DGraphPrivate(this))
{
    setAutoReplot( false );
    setAutoFillBackground(true);

    QwtPlotCanvas* pCanvas = new QwtPlotCanvas();
    //pCanvas->setLineWidth( 0 );
    pCanvas->setAutoFillBackground(true);
    pCanvas->setFrameStyle( QFrame::Box );
    //pCanvas->setBorderRadius( 0 );//设置圆角为0

    QPalette canvasPalette( Qt::white );
    //canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    pCanvas->setPalette( canvasPalette );
    setCanvas( pCanvas );
	setPalette( Qt::white );
    setAutoReplot( true );
    canvas()->setCursor(Qt::ArrowCursor);
}

SA2DGraph::~SA2DGraph()
{

}

void SA2DGraph::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
	
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11
    //updateGradient();
}

QwtPlotZoomer *SA2DGraph::zoomer()
{
    SA_D(SA2DGraph);
    return d->m_zoomer.data();
}

QwtPlotZoomer *SA2DGraph::zoomerSecond()
{
    SA_D(SA2DGraph);
    return d->m_zoomerSecond.data();
}

QwtPlotGrid *SA2DGraph::grid()
{
    SA_D(SA2DGraph);
    return d->m_grid;
}


bool SA2DGraph::isEnableGrid() const
{
    SA_DC(SA2DGraph);
    if(d->m_grid)
        return d->m_grid->isVisible();
    return false;
}

bool SA2DGraph::isEnableGridX() const
{
    SA_DC(SA2DGraph);
    if(d->m_grid)
        if(d->m_grid->isVisible())
            return d->m_grid->xEnabled();
    return false;
}

bool SA2DGraph::isEnableGridY() const
{
    SA_DC(SA2DGraph);
    if(d->m_grid)
        if(d->m_grid->isVisible())
            return d->m_grid->yEnabled();
    return false;
}

bool SA2DGraph::isEnableGridXMin() const
{
    SA_DC(SA2DGraph);
    if(d->m_grid)
        if(d->m_grid->isVisible())
            return d->m_grid->xMinEnabled();
    return false;
}

bool SA2DGraph::isEnableGridYMin() const
{
    SA_DC(SA2DGraph);
    if(d->m_grid)
        if(d->m_grid->isVisible())
            return d->m_grid->yMinEnabled();
    return false;
}

bool SA2DGraph::isEnablePanner() const
{
    SA_DC(SA2DGraph);
    if(d->m_panner)
        return d->m_panner->isEnabled();
    return false;
}

bool SA2DGraph::isEnableLegend() const
{
    SA_DC(SA2DGraph);
    if(d->m_legend)
        return d->m_legend->isVisible();
    return false;
}

bool SA2DGraph::isEnableLegendPanel() const
{
    SA_DC(SA2DGraph);
    if(d->m_legendPanel)
        return d->m_legendPanel->isVisible();
    return false;

}

bool SA2DGraph::isEnableYDataPicker() const
{
    SA_DC(SA2DGraph);
    if(d->m_yDataPicker)
        return d->m_yDataPicker->isEnabled();
    return false;
}

bool SA2DGraph::isEnableXYDataPicker() const
{
    SA_DC(SA2DGraph);
    if(d->m_xyDataPicker)
        return d->m_xyDataPicker->isEnabled();
    return false;
}
//========================================================================================
//网格 grid 操作
//========================================================================================
QwtPlotGrid* SA2DGraph::setupGrid(
        const QColor & 	color
        ,qreal 	width
        ,Qt::PenStyle style)
{
    SA_D(SA2DGraph);
    bool bShouldAttachAgain(false);
    if(nullptr == d->m_grid){
        d->m_grid = new QwtPlotGrid;
        bShouldAttachAgain = true;
    }
    //大刻度显示网格- 所谓大刻度是值刻度上带数值的
    d->m_grid->setMajorPen( color, width, style );
    d->m_grid->setMinorPen( color, 0 , Qt::DotLine );//小刻度的样式
    if(bShouldAttachAgain){
        d->m_grid->attach( this );
    }
    return d->m_grid;
}

void SA2DGraph::deleteGrid(){
    SA_D(SA2DGraph);
    if(nullptr == d->m_grid){
        return;
    }
    d->m_grid->detach();
    delete d->m_grid;
    d->m_grid = nullptr;
    replot();//刷新，否则不显示
}

void SA2DGraph::enableGrid(bool isShow)
{
    SA_D(SA2DGraph);
	if (isShow)
	{
        if(nullptr == d->m_grid){
			setupGrid();
		}
        d->m_grid->enableX(true);
        d->m_grid->enableY(true);
        d->m_grid->show();
		emit enableGridXChanged(isShow);
		emit enableGridYChanged(isShow);
		emit enableGridChanged(isShow);
		return;
	}
	else
	{
        if(nullptr == d->m_grid){
			return;
		}
        d->m_grid->hide();
	}
    replot();
	emit enableGridChanged(isShow);
}

void SA2DGraph::enableGridX(bool enable)
{
    SA_D(SA2DGraph);
    if (nullptr == d->m_grid)
		return;
    d->m_grid->enableX(enable);
	emit enableGridXChanged(enable);
	if (!enable)
	{
		emit enableGridXMinChanged(false);
	}
	//m_grid->show();//刷新
}

void SA2DGraph::enableGridY(bool enable)
{
    SA_D(SA2DGraph);
    if (nullptr == d->m_grid)
		return;
    d->m_grid->enableY(enable);
	emit enableGridYChanged(enable);
	if (!enable)
	{
		emit enableGridYMinChanged(false);
	}
}

void SA2DGraph::enableGridXMin(bool enable)
{
    SA_D(SA2DGraph);
    if (nullptr == d->m_grid)
		return;
    d->m_grid->enableXMin(enable);
	emit enableGridXMinChanged(enable);
}
void SA2DGraph::enableGridYMin(bool enable)
{
    SA_D(SA2DGraph);
    if (nullptr == d->m_grid)
		return;
    d->m_grid->enableYMin(enable);
	emit enableGridYMinChanged(enable);
}

//========================================================================================
//画线和数据 操作
//========================================================================================


///
/// \brief 设置y值横线标记
/// \param data 值
/// \param strLabel 描述
/// \param clr 颜色
/// \todo type show be use
///
void SA2DGraph::markYValue(double data, const QString &strLabel, QColor clr)
{
    double x = axisXmax();
    SAYValueMarker* valueMark = new SAYValueMarker(data);
    valueMark->setXValue(x);
    valueMark->setLinePen(clr,1);
    valueMark->setLabel(strLabel);
    valueMark->setLabelAlignment(Qt::AlignTop|Qt::AlignRight);
    valueMark->setSpacing(1);//设置文字和mark的间隔
    valueMark->attach( this );
}


double SA2DGraph::axisXmin(int axisId) const
{
    QwtInterval inl = axisInterval(axisId);
    if(inl.isValid())
        return inl.minValue();
    axisId = ((axisId == QwtPlot::xBottom) ? QwtPlot::xTop : QwtPlot::xBottom);
    inl = axisInterval(axisId);
    if(inl.isValid())
        return inl.minValue();
    return double();
}

double SA2DGraph::axisXmax(int axisId) const
{
    QwtInterval inl = axisInterval(axisId);
    if(inl.isValid())
        return inl.maxValue();
    axisId = ((axisId == QwtPlot::xBottom) ? QwtPlot::xTop : QwtPlot::xBottom);
    inl = axisInterval(axisId);
    if(inl.isValid())
        return inl.maxValue();
    return double();
}

double SA2DGraph::axisYmin(int axisId) const
{
    QwtInterval inl = axisInterval(axisId);
    if(inl.isValid())
        return inl.minValue();
    axisId = ((axisId == QwtPlot::yLeft) ? QwtPlot::yRight : QwtPlot::yLeft);
    inl = axisInterval(axisId);
    if(inl.isValid())
        return inl.minValue();
    return double();
}

double SA2DGraph::axisYmax(int axisId) const
{
    QwtInterval inl = axisInterval(axisId);
    if(inl.isValid())
        return inl.maxValue();
    axisId = ((axisId == QwtPlot::yLeft) ? QwtPlot::yRight : QwtPlot::yLeft);
    inl = axisInterval(axisId);
    if(inl.isValid())
        return inl.maxValue();
    return double();
}
///
/// \brief 此功能用于禁止所有活动的editor，如Zoomer，Picker，Panner，DataPicker等
///
void SA2DGraph::setEnableAllEditor(bool enable)
{
    if(isEnableZoomer())
    {
        enableZoomer(enable);
    }
    if(isEnablePicker())
    {
        enablePicker(enable);
    }
    if(isEnablePanner())
    {
        enablePanner(enable);
    }
    if(isEnableYDataPicker())
    {
        enableYDataPicker(enable);
    }
    if(isEnableXYDataPicker())
    {
        enableXYDataPicker(enable);
    }
}


void SA2DGraph::setupPicker()
{
    SA_D(SA2DGraph);
    if(nullptr == d->m_picker)
    {
        d->m_picker = new SACrossTracker(this->canvas());
    }

}

void SA2DGraph::enablePicker(bool enable)
{
    SA_D(SA2DGraph);
    if(!enable && nullptr == d->m_picker)
        return;
    if(nullptr == d->m_picker)
        setupPicker();
    d->m_picker->setEnabled( enable );
    if(d->m_zoomer.isNull())
    {
        if(isEnableZoomer())
        {
            d->m_zoomer->setTrackerMode( (enable ? QwtPicker::AlwaysOff : QwtPicker::AlwaysOn) );
        }
    }
	emit enablePickerChanged(enable);
}


void SA2DGraph::enablePanner(bool enable)
{
    SA_D(SA2DGraph);
	if (enable)
	{
        if (nullptr == d->m_panner)
		{
			setupPanner();
		}
        d->m_panner->setEnabled(enable);
	}
	else
	{
        if (nullptr != d->m_panner)
		{
            d->m_panner->setEnabled(enable);
		}
	}
	emit enablePannerChanged(enable);
}
///
/// \brief 建立一个内置的Panner(拖动)，默认使用鼠标中键
///
void SA2DGraph::setupPanner()
{
	//设置拖动
    SA_D(SA2DGraph);
    if (nullptr == d->m_panner)
	{
        d->m_panner = new QwtPlotPanner( canvas() );
        d->m_panner->setCursor(QCursor(Qt::ClosedHandCursor));
        d->m_panner->setMouseButton( Qt::LeftButton );
	}
}
void SA2DGraph::deletePanner()
{
    SA_D(SA2DGraph);
    if (nullptr != d->m_panner)
	{
        d->m_panner->setParent(nullptr);//断开和canvas()的父子连接
        delete d->m_panner;
        d->m_panner = nullptr;
	}
}
///
/// \brief 允许缩放
/// \param enable
///
void SA2DGraph::enableZoomer(bool enable)
{
    SA_D(SA2DGraph);
    if(!enable)
    {
        if(d->m_zoomer.isNull())
        {
            return;
        }
    }
    if(d->m_zoomer.isNull() /*|| nullptr == m_zoomerSecond*/)
    {
        setupZoomer();
    }
    enableZoomer(d->m_zoomer.data(),enable);
    enableZoomer(d->m_zoomerSecond.data(),enable);
    if(isEnablePicker())
    {
        d->m_zoomer->setTrackerMode( (enable ? QwtPicker::AlwaysOff : QwtPicker::ActiveOnly) );
    }
    emit enableZoomerChanged(enable);
}

void SA2DGraph::enableZoomer(QwtPlotZoomer *zoomer,bool enable)
{
    if(nullptr == zoomer)
    {
        return;
    }
    if(enable)
    {
        zoomer->setEnabled(true);
        zoomer->setZoomBase(true);
        zoomer->setRubberBand(QwtPicker::RectRubberBand);
        zoomer->setTrackerMode( (isEnablePicker() ? QwtPicker::AlwaysOff : QwtPicker::ActiveOnly) );

    }
    else
    {
        zoomer->setEnabled(false);
        zoomer->setRubberBand(QwtPicker::NoRubberBand);
        zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    }
    if(isEnablePicker())
    {
        zoomer->setTrackerMode( (enable ? QwtPicker::AlwaysOff : QwtPicker::ActiveOnly) );
    }
}
///
/// \brief 回到放大的最底栈
///
void SA2DGraph::setZoomBase()
{
    SA_D(SA2DGraph);
    if(!d->m_zoomer.isNull())
    {
        d->m_zoomer->setZoomBase(true);
    }
    if(!d->m_zoomerSecond.isNull())
    {
        d->m_zoomerSecond->setZoomBase(true);
    }
}

void SA2DGraph::setupZoomer()
{
    SA_D(SA2DGraph);
    if(d->m_zoomer.isNull())
    {
        qDebug()<<"setup zoom";
#if 0
        Zoomer_qwt* zoom = new Zoomer_qwt(xBottom,yLeft, canvas());//Zoomer_qwt( QwtPlot::xBottom, QwtPlot::yLeft,canvas() );

        zoom->on_enable_scrollBar(isHaveScroll);
        zoom->setRubberBand( QwtPicker::RectRubberBand );
        zoom->setTrackerPen( QColor( Qt::black ) );
        zoom->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
        zoom->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
        zoom->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );
        zoom->setMousePattern( QwtEventPattern::MouseSelect2,
                Qt::RightButton, Qt::ControlModifier );
        zoom->setMousePattern( QwtEventPattern::MouseSelect3,
                Qt::RightButton );
        zoom->setTrackerMode( QwtPicker::AlwaysOff );
        m_zoomer = zoom;

#else
        d->m_zoomer.reset(new SAPlotZoomer(xBottom,yLeft,canvas()));
        d->m_zoomer->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
        d->m_zoomer->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
        d->m_zoomer->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );
        d->m_zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
                Qt::RightButton, Qt::ControlModifier );
        d->m_zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
                Qt::RightButton );
        d->m_zoomer->setTrackerMode( QwtPicker::AlwaysOff );
        d->m_zoomer->setZoomBase(false);
        d->m_zoomer->setMaxStackDepth(20);
#endif
    }
    if(nullptr == d->m_zoomerSecond)
    {
        d->m_zoomerSecond.reset(new SAPlotZoomer(xTop,yRight,canvas()));
        d->m_zoomerSecond->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
        d->m_zoomerSecond->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
        d->m_zoomerSecond->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );
        d->m_zoomerSecond->setMousePattern( QwtEventPattern::MouseSelect2,
                Qt::RightButton, Qt::ControlModifier );
        d->m_zoomerSecond->setMousePattern( QwtEventPattern::MouseSelect3,
                Qt::RightButton );
        d->m_zoomerSecond->setTrackerMode( QwtPicker::AlwaysOff );
        d->m_zoomerSecond->setZoomBase(false);
        d->m_zoomerSecond->setMaxStackDepth(20);
    }
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );
    magnifier->setMouseButton( Qt::NoButton );

}

void SA2DGraph::deleteZoomer()
{
    SA_D(SA2DGraph);
    if(!d->m_zoomer.isNull())
    {
        d->m_zoomer.reset();
    }
    if(!d->m_zoomerSecond.isNull())
    {
        d->m_zoomerSecond.reset();
    }
}



///
/// \brief 设置缩放重置
///
void SA2DGraph::setZoomReset()
{
    SA_D(SA2DGraph);
    if(!d->m_zoomer.isNull())
    {
        d->m_zoomer->zoom(0);

    }
    if(!d->m_zoomerSecond.isNull())
    {
        d->m_zoomerSecond->zoom(0);
    }
}

void SA2DGraph::zoomIn()
{
    SA_D(SA2DGraph);
    if(d->m_zoomer.isNull())
    {
        setupZoomer();
    }

    QRectF rect = d->m_zoomer->zoomRect();
    double w = rect.width()*0.625;
    double h = rect.height()*0.625;
    double x = rect.x() + (rect.width()-w)/2.0;
    double y = rect.y() + (rect.height()-h)/2.0;
    rect.setX(x);
    rect.setY(y);
    rect.setWidth(w);
    rect.setHeight(h);

    d->m_zoomer->zoom(rect);
}

void SA2DGraph::zoomOut()
{
    SA_D(SA2DGraph);
    if(d->m_zoomer.isNull())
    {
        setupZoomer();
    }

    QRectF rect = d->m_zoomer->zoomRect();
    double w = rect.width()*1.6;
    double h = rect.height()*1.6;
    double x = rect.x() - (w - rect.width())/2.0;
    double y = rect.y() - (h - rect.height())/2.0;
    rect.setX(x);
    rect.setY(y);
    rect.setWidth(w);
    rect.setHeight(h);
    d->m_zoomer->zoom(rect);

}

///
/// \brief 缩放到最适合比例，就是可以把所有图都能看清的比例
///
void SA2DGraph::zoomInCompatible()
{
    SA_D(SA2DGraph);
    QwtInterval intv[axisCnt];
    SAChart::dataRange(this,&intv[yLeft],&intv[yRight],&intv[xBottom],&intv[xTop]);
    if(!d->m_zoomer.isNull())
    {
        int axx = d->m_zoomer->xAxis();
        int axy = d->m_zoomer->yAxis();
        QRectF rect1;
        rect1.setRect(intv[axx].minValue(),intv[axy].minValue(),intv[axx].width(),intv[axy].width());
        qDebug()<<rect1;
        d->m_zoomer->zoom(rect1);
    }
    if(!d->m_zoomerSecond.isNull())
    {
        int axx = d->m_zoomerSecond->xAxis();
        int axy = d->m_zoomerSecond->yAxis();
        QRectF rect1;
        rect1.setRect(intv[axx].minValue(),intv[axy].minValue(),intv[axx].width(),intv[axy].width());
        qDebug()<<rect1;
        d->m_zoomerSecond->zoom(rect1);
    }
    /* !此方法不行
    if(!d->m_zoomer.isNull())
    {
        int axx = d->m_zoomer->xAxis();
        int axy = d->m_zoomer->yAxis();
        double xmin = axisScaleEngine(axx)->lowerMargin();
        double xmax = axisScaleEngine(axx)->upperMargin();
        double ymin = axisScaleEngine(axy)->lowerMargin();
        double ymax = axisScaleEngine(axy)->upperMargin();
        QRectF rect1;
        rect1.setRect(xmin,ymin,xmax-xmin,ymax-ymin);
        qDebug()<<rect1;
        d->m_zoomer->zoom(rect1);
    }
    */
}

void SA2DGraph::setupLegend()
{
    SA_D(SA2DGraph);
    if (nullptr == d->m_legend)
	{
        d->m_legend = new LegendItem();
        d->m_legend->attach( this );
	}
}

void SA2DGraph::enableLegend(bool enable)
{
    SA_D(SA2DGraph);
	if (enable)
	{
        if (d->m_legend)
		{
            d->m_legend->setVisible(true);
		}
		else
		{
			setupLegend();
		}
	}
	else
	{
        if (d->m_legend)
		{
            d->m_legend->setVisible(false);
// 			m_legend->detach();
// 			delete m_legend;
// 			m_legend = nullptr;
		}
	}
	emit enableLegendChanged(enable);
}

void SA2DGraph::enableLegendPanel(bool enable )
{
	if (enable)
	{
		setuplegendPanel();	
	}
	else
	{
		deletelegendPanel();
	}
	emit enableLegendPanelChanged(enable);
}
void SA2DGraph::setuplegendPanel()
{
    SA_D(SA2DGraph);
    if (d->m_legendPanel)
	{
		return;
	}
    d->m_legendPanel = new QwtLegend;
    d->m_legendPanel->setDefaultItemMode( QwtLegendData::Checkable );
    insertLegend( d->m_legendPanel, QwtPlot::RightLegend );
//	connect( m_legendPanel, &QwtLegend::checked,&ChartWave_qwt::showItem);
    connect( d->m_legendPanel, SIGNAL( checked( const QVariant &, bool, int ) ),
		SLOT( showItem( const QVariant &, bool ) ) );

	QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve );
	for ( int i = 0; i < items.size(); i++ )
	{
		const QVariant itemInfo = itemToInfo( items[i] );
		QwtLegendLabel *legendLabel =
            qobject_cast<QwtLegendLabel *>( d->m_legendPanel->legendWidget( itemInfo ) );
		if ( legendLabel )
			legendLabel->setChecked( items[i]->isVisible() );
	}
}
void SA2DGraph::deletelegendPanel()
{
    SA_D(SA2DGraph);
	insertLegend( nullptr);//内部会销毁
    d->m_legendPanel = nullptr;
}

void SA2DGraph::setupYDataPicker()
{
    SA_D(SA2DGraph);
    if (nullptr == d->m_yDataPicker)
	{
        d->m_yDataPicker = new SAYDataTracker( this->canvas() );
        d->m_yDataPicker->setRubberBandPen( QPen( "MediumOrchid" ) );
	}
}
void SA2DGraph::deleteYDataPicker()
{
    SA_D(SA2DGraph);
    if (nullptr != d->m_yDataPicker)
	{
        d->m_yDataPicker->setParent(nullptr);//断开和canvas()的父子连接
        delete d->m_yDataPicker;
        d->m_yDataPicker = nullptr;
    }
}

void SA2DGraph::setupXYDataPicker()
{
    SA_D(SA2DGraph);
    if (nullptr == d->m_xyDataPicker)
    {
        d->m_xyDataPicker = new SAXYDataTracker( this->canvas() );
        d->m_xyDataPicker->setRubberBandPen( QPen( "MediumOrchid" ) );
    }
}

void SA2DGraph::deleteXYDataPicker()
{
    SA_D(SA2DGraph);
    if (nullptr != d->m_xyDataPicker)
    {
        d->m_xyDataPicker->setParent(nullptr);//断开和canvas()的父子连接
        delete d->m_xyDataPicker;
        d->m_xyDataPicker = nullptr;
    }
}

void SA2DGraph::enableYDataPicker(bool enable)
{
	if (enable)
	{
        setupYDataPicker();
	}
	else
	{
        deleteYDataPicker();
	}
    emit enableYDataPickerChanged(enable);
}

void SA2DGraph::enableXYDataPicker(bool enable)
{
    if (enable)
    {
        setupXYDataPicker();
    }
    else
    {
        deleteXYDataPicker();
    }
    emit enableXYDataPickerChanged(enable);
}
///
/// \brief 是否允许缩放
/// \return
///
bool SA2DGraph::isEnableZoomer() const
{
    SA_DC(SA2DGraph);
    if(d->m_zoomer)
        return d->m_zoomer->isEnabled();
    if(d->m_zoomerSecond)
        return d->m_zoomerSecond->isEnabled();
    return false;
}
///
/// \brief 是否允许十字光标
/// \return
///
bool SA2DGraph::isEnablePicker() const
{
    SA_DC(SA2DGraph);
    if(d->m_picker)
        return d->m_picker->isEnabled();
    return false;
}
void SA2DGraph::showItem( const QVariant &itemInfo, bool on )
{
	QwtPlotItem *plotItem = infoToItem( itemInfo );
	if ( plotItem )
		plotItem->setVisible( on );
}

QList<QwtPlotCurve*> SA2DGraph::getCurveList()
{
    QList<QwtPlotCurve*> curves;
    QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotCurve);
    for(int i(0);i<items.size();++i){
        curves.append(static_cast<QwtPlotCurve*>( items[i] ));
    }
    return curves;
}
///
/// \brief getMakerList 获取所有标记
/// \return
///
QList<QwtPlotMarker*> SA2DGraph::getMakerList()
{
    QList<QwtPlotMarker*> list;
    QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotMarker);
    for(int i(0);i<items.size();++i){
        list.append(static_cast<QwtPlotMarker*>( items[i] ));
    }
    return list;
}




QwtDateScaleDraw* SA2DGraph::setAxisDateTimeScale(const QString &format, int axisID , QwtDate::IntervalType intType)
{
    QwtDateScaleDraw* dateScale = setAxisDateTimeScale(axisID);
    dateScale->setDateFormat(intType,format);
    return dateScale;
}

QwtDateScaleDraw *SA2DGraph::setAxisDateTimeScale(int axisID)
{
    QwtDateScaleDraw* dateScale;
    dateScale = new QwtDateScaleDraw;//原来的scaleDraw会再qwt自动delete
    setAxisScaleDraw(axisID,dateScale);
    return dateScale;
}

