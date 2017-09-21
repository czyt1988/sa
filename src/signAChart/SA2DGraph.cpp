#include "SA2DGraph.h"
#include "SAChart.h"
#include <qstyle.h>
#include <qstyleoption.h>

#include <qwt_interval.h>
#include <qwt_picker_machine.h>
#include <algorithm>
#include <qwt_legend_label.h>
#include <qwt_date_scale_draw.h>

#include "SAPlotMarker.h"
#include "SAYDataTracker.h"
#include "SAXYDataTracker.h"
#include "SACrossTracker.h"
//unsigned int ChartWave_qwt::staticValue_nAutoLineID = 0;//静态变量初始化



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
  ,m_grid(nullptr)
  ,m_picker(nullptr)
  ,m_zoomer(nullptr)
  ,m_zoomerSecond(nullptr)
  ,m_panner(nullptr)
  ,m_legend(nullptr)
  ,m_legendPanel(nullptr)
  ,m_yDataPicker(nullptr)
  ,m_xyDataPicker(nullptr)
{
    setAutoReplot( false );
    setAutoFillBackground(true);

    QwtPlotCanvas* pCanvas = new QwtPlotCanvas();
    //pCanvas->setLineWidth( 0 );
	pCanvas->setAutoFillBackground(false);
    pCanvas->setFrameStyle( QFrame::NoFrame );
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

//bool SA2DGraph::isEnableZoomerScroll() const
//{
//    Zoomer_qwt* zm = qobject_cast<Zoomer_qwt*>(m_zoomer);
//    if(zm)
//    {
//        return zm->isEnableScrollBar();
//    }
//    return false;
//}

bool SA2DGraph::isEnableGrid() const
{
    if(m_grid)
        return m_grid->isVisible();
    return false;
}

bool SA2DGraph::isEnableGridX() const
{
    if(m_grid)
        if(m_grid->isVisible())
            return m_grid->xEnabled();
    return false;
}

bool SA2DGraph::isEnableGridY() const
{
    if(m_grid)
        if(m_grid->isVisible())
            return m_grid->yEnabled();
    return false;
}

bool SA2DGraph::isEnableGridXMin() const
{
    if(m_grid)
        if(m_grid->isVisible())
            return m_grid->xMinEnabled();
    return false;
}

bool SA2DGraph::isEnableGridYMin() const
{
    if(m_grid)
        if(m_grid->isVisible())
            return m_grid->yMinEnabled();
    return false;
}

bool SA2DGraph::isEnablePanner() const
{
    if(m_panner)
        return m_panner->isEnabled();
    return false;
}

bool SA2DGraph::isEnableLegend() const
{
    if(m_legend)
        return m_legend->isVisible();
    return false;
}

bool SA2DGraph::isEnableLegendPanel() const
{
    if(m_legendPanel)
        return m_legendPanel->isVisible();
    return false;

}

bool SA2DGraph::isEnableYDataPicker() const
{
    if(m_yDataPicker)
        return m_yDataPicker->isEnabled();
    return false;
}

bool SA2DGraph::isEnableXYDataPicker() const
{
    if(m_xyDataPicker)
        return m_xyDataPicker->isEnabled();
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
    bool bShouldAttachAgain(false);
    if(nullptr == m_grid){
        m_grid = new QwtPlotGrid;
        bShouldAttachAgain = true;
    }
    //大刻度显示网格- 所谓大刻度是值刻度上带数值的
    m_grid->setMajorPen( color, width, style );
	m_grid->setMinorPen( color, 0 , Qt::DotLine );//小刻度的样式
    if(bShouldAttachAgain){
        m_grid->attach( this );
    }
    return m_grid;
}

void SA2DGraph::deleteGrid(){
    if(nullptr == m_grid){
        return;
    }
    m_grid->detach();
    delete m_grid;
    m_grid = nullptr;
    replot();//刷新，否则不显示
}

void SA2DGraph::enableGrid(bool isShow)
{
	if (isShow)
	{
		if(nullptr == m_grid){
			setupGrid();
		}
		m_grid->enableX(true);
		m_grid->enableY(true);
		m_grid->show();
		emit enableGridXChanged(isShow);
		emit enableGridYChanged(isShow);
		emit enableGridChanged(isShow);
		return;
	}
	else
	{
		if(nullptr == m_grid){
			return;
		}
		m_grid->hide();
	}
    replot();
	emit enableGridChanged(isShow);
}

void SA2DGraph::enableGridX(bool enable)
{
	if (nullptr == m_grid)
		return;
	m_grid->enableX(enable);
	emit enableGridXChanged(enable);
	if (!enable)
	{
		emit enableGridXMinChanged(false);
	}
	//m_grid->show();//刷新
}

void SA2DGraph::enableGridY(bool enable)
{
	if (nullptr == m_grid)
		return;
	m_grid->enableY(enable);
	emit enableGridYChanged(enable);
	if (!enable)
	{
		emit enableGridYMinChanged(false);
	}
}

void SA2DGraph::enableGridXMin(bool enable)
{
	if (nullptr == m_grid)
		return;
	m_grid->enableXMin(enable);
	emit enableGridXMinChanged(enable);
}
void SA2DGraph::enableGridYMin(bool enable)
{
	if (nullptr == m_grid)
		return;
	m_grid->enableYMin(enable);
	emit enableGridYMinChanged(enable);
}

//========================================================================================
//画线和数据 操作
//========================================================================================

QwtPlotCurve* SA2DGraph::addCurve(const QVector<QPointF>& xyDatas)
{
    QwtPlotCurve* pCurve = nullptr;
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(xyDatas);
    pCurve->attach(this);
    emit plotCurveChanged(pCurve);
    return pCurve;
}

QwtPlotCurve* SA2DGraph::addCurve(std::vector<double>& xDatas,std::vector<double>& yDatas)
{
    QwtPlotCurve* pCurve = nullptr;
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(&xDatas[0],&yDatas[0],xDatas.size() <= yDatas.size() ? xDatas.size() : yDatas.size());
    pCurve->attach(this);
    emit plotCurveChanged(pCurve);
    return pCurve;
}

QwtPlotCurve* SA2DGraph::addCurve(const double *xData, const double *yData, int size)
{
    QwtPlotCurve* pCurve = nullptr;
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(xData,yData,size);
    pCurve->attach(this);
    emit plotCurveChanged(pCurve);
    return pCurve;
}

QwtPlotCurve* SA2DGraph::addCurve(const QVector< double > &xData
                                      , const QVector< double > &yData)
{
    QwtPlotCurve* pCurve = nullptr;
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(xData,yData);
    pCurve->attach(this);
    emit plotCurveChanged(pCurve);
    return pCurve;
}


void SA2DGraph::addCurve(QwtPlotCurve* pC)
{
    pC->attach(this);
    emit plotCurveChanged(pC);
}
///
/// \brief 添加一条竖直线
/// \return
///
QwtPlotMarker* SA2DGraph::addVLine(double val)
{
    QwtPlotMarker *marker = new QwtPlotMarker();
    marker->setXValue(val);
    marker->setLineStyle( QwtPlotMarker::VLine );
    marker->setItemAttribute( QwtPlotItem::Legend, true );
    marker->attach( this );
    return marker;
}
QwtPlotMarker* SA2DGraph::addHLine(double val)
{
    QwtPlotMarker *marker = new QwtPlotMarker();
    marker->setYValue(val);
    marker->setLineStyle( QwtPlotMarker::HLine );
    marker->setItemAttribute( QwtPlotItem::Legend, true );
    marker->attach( this );
    return marker;
}

QwtPlotHistogram*SA2DGraph::addBar(const QVector<QwtIntervalSample>& sample)
{
    QwtPlotHistogram* his = new QwtPlotHistogram();
    his->setSamples (sample);
    his->setYAxis(yLeft);
    his->setXAxis(xBottom);
    his->setStyle (QwtPlotHistogram::Columns);
    his->attach (this);
    emit plotCurveChanged(his);
    return his;
}

///
/// \brief 在图片上标记点
/// \param pos 点的位置
/// \param strLabel 说明
/// \param type 样式，默认为0，就是箭头
/// \todo type and clr show be use
///
void SA2DGraph::markPoint(QPointF pos, const QString& strLabel, QColor clr, int type)
{
    SAPointMarker* pointMark = new SAPointMarker(pos);
    SATriangleMarkSymbol* tra = new SATriangleMarkSymbol();
    pointMark->setSymbol( tra );
    pointMark->setLabel(strLabel);
    pointMark->setLabelAlignment(Qt::AlignTop|Qt::AlignHCenter);
    pointMark->setSpacing(10);//设置文字和mark的间隔
    pointMark->attach( this );

    //TODO
    ///未完成，注意
}

///
/// \brief 设置y值横线标记
/// \param data 值
/// \param strLabel 描述
/// \param clr 颜色
/// \param type
/// \todo type show be use
///
void SA2DGraph::markYValue(double data, const QString &strLabel, QColor clr, int type)
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
///
/// \brief 擦除所有的标记
///
void SA2DGraph::removeAllMarker()
{
    detachItems(QwtPlotItem::Rtti_PlotMarker);
    QList<QwtPlotItem*> items = itemList();
    for(int i=0;i<items.size();++i)
    {
        if (items[i]->rtti() > SARttiMarker_LowerBoundary
                &&
                items[i]->rtti() < SARttiMarker_UpperBoundary
                )
        {
           items[i]->detach();
           if(items[i])
               delete items[i];
        }
    }
}
///
/// \brief 根据给定的item进行删除
/// \param markRtti 标记要删除的item的rtti
///
void SA2DGraph::deleteItems(const QList<int> markRtti)
{
    QList<QwtPlotItem*> items = itemList();
    for(auto i=items.begin();i!=items.end();++i)
    {
        if(markRtti.contains((*i)->rtti()))
        {
            if((*i))
            {
                (*i)->detach();
                delete (*i);
            }
        }
    }
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


void SA2DGraph::setupPicker()
{
    if(nullptr == m_picker)
    {
        m_picker = new SACrossTracker(this->canvas());
    }

}

void SA2DGraph::enablePicker(bool enable)
{
    if(!enable && nullptr == m_picker)
        return;
    if(nullptr == m_picker)
        setupPicker();
	m_picker->setEnabled( enable );
    if(m_zoomer.isNull())
    {
        if(isEnableZoomer())
        {
            m_zoomer->setTrackerMode( (enable ? QwtPicker::AlwaysOff : QwtPicker::AlwaysOn) );
        }
    }
	emit enablePickerChanged(enable);
}


void SA2DGraph::enablePanner(bool enable)
{
	if (enable)
	{
		if (nullptr == m_panner)
		{
			setupPanner();
		}
		m_panner->setEnabled(enable);
	}
	else
	{
		if (nullptr != m_panner)
		{
			m_panner->setEnabled(enable);
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
	if (nullptr == m_panner)
	{
		m_panner = new QwtPlotPanner( canvas() );
		m_panner->setMouseButton( Qt::MidButton );
	}
}
void SA2DGraph::deletePanner()
{
	if (nullptr != m_panner)
	{
		m_panner->setParent(nullptr);//断开和canvas()的父子连接
		delete m_panner;
		m_panner = nullptr;
	}
}
///
/// \brief 允许缩放
/// \param enable
///
void SA2DGraph::enableZoomer(bool enable)
{
    if(!enable)
    {
        if(m_zoomer.isNull())
        {
            return;
        }
    }
    if(m_zoomer.isNull() /*|| nullptr == m_zoomerSecond*/)
    {
        setupZoomer();
    }
    enableZoomer(m_zoomer.data(),enable);
    enableZoomer(m_zoomerSecond.data(),enable);
    if(isEnablePicker())
    {
        m_zoomer->setTrackerMode( (enable ? QwtPicker::AlwaysOff : QwtPicker::ActiveOnly) );
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
    if(!m_zoomer.isNull())
    {
        m_zoomer->setZoomBase(true);
    }
    if(!m_zoomerSecond.isNull())
    {
        m_zoomerSecond->setZoomBase(true);
    }
}

void SA2DGraph::setupZoomer()
{
    if(m_zoomer.isNull())
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
        m_zoomer.reset(new SAPlotZoomer(xBottom,yLeft,canvas()));
        m_zoomer->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
        m_zoomer->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
        m_zoomer->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );
        m_zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
                Qt::RightButton, Qt::ControlModifier );
        m_zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
                Qt::RightButton );
        m_zoomer->setTrackerMode( QwtPicker::AlwaysOff );
        m_zoomer->setZoomBase(false);
        m_zoomer->setMaxStackDepth(20);
#endif
    }
    if(nullptr == m_zoomerSecond)
    {
        m_zoomerSecond.reset(new SAPlotZoomer(xTop,yRight,canvas()));
        m_zoomerSecond->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
        m_zoomerSecond->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
        m_zoomerSecond->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );
        m_zoomerSecond->setMousePattern( QwtEventPattern::MouseSelect2,
                Qt::RightButton, Qt::ControlModifier );
        m_zoomerSecond->setMousePattern( QwtEventPattern::MouseSelect3,
                Qt::RightButton );
        m_zoomerSecond->setTrackerMode( QwtPicker::AlwaysOff );
        m_zoomerSecond->setZoomBase(false);
        m_zoomerSecond->setMaxStackDepth(20);
    }
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );
    magnifier->setMouseButton( Qt::NoButton );

}

void SA2DGraph::deleteZoomer()
{
    if(!m_zoomer.isNull())
    {
        m_zoomer.reset();
    }
    if(!m_zoomerSecond.isNull())
    {
        m_zoomerSecond.reset();
    }
}



///
/// \brief 设置缩放重置
///
void SA2DGraph::setZoomReset()
{
//    setAxisAutoScale(QwtPlot::yLeft,true);
//    setAxisAutoScale(QwtPlot::xBottom,true);
//    setAxisAutoScale(QwtPlot::xTop,true);
//    setAxisAutoScale(QwtPlot::yRight,true);
//    replot();
    if(!m_zoomer.isNull())
    {
        m_zoomer->setZoomBase(false);
        m_zoomer->zoom(0);

    }
    if(m_zoomerSecond)
    {
        m_zoomerSecond->setZoomBase(false);
        m_zoomerSecond->zoom(0);
    }

}

void SA2DGraph::zoomIn()
{
    if(m_zoomer.isNull())
    {
        setupZoomer();
    }

    QRectF rect = m_zoomer->zoomRect();
    double w = rect.width()*0.625;
    double h = rect.height()*0.625;
    double x = rect.x() + (rect.width()-w)/2.0;
    double y = rect.y() + (rect.height()-h)/2.0;
    rect.setX(x);
    rect.setY(y);
    rect.setWidth(w);
    rect.setHeight(h);

    m_zoomer->zoom(rect);
}

void SA2DGraph::zoomOut()
{
    if(m_zoomer.isNull())
    {
        setupZoomer();
    }

    QRectF rect = m_zoomer->zoomRect();
    double w = rect.width()*1.6;
    double h = rect.height()*1.6;
    double x = rect.x() - (w - rect.width())/2.0;
    double y = rect.y() - (h - rect.height())/2.0;
    rect.setX(x);
    rect.setY(y);
    rect.setWidth(w);
    rect.setHeight(h);
    m_zoomer->zoom(rect);

}

void SA2DGraph::setupLegend()
{
	if (nullptr == m_legend)
	{
		m_legend = new LegendItem();
		m_legend->attach( this );
	}
}

void SA2DGraph::enableLegend(bool enable)
{
	if (enable)
	{
		if (m_legend)
		{
			m_legend->setVisible(true);
		}
		else
		{
			setupLegend();
		}
	}
	else
	{
		if (m_legend)
		{
			m_legend->setVisible(false);
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
	if (m_legendPanel)
	{
		return;
	}
	m_legendPanel = new QwtLegend;
	m_legendPanel->setDefaultItemMode( QwtLegendData::Checkable );
	insertLegend( m_legendPanel, QwtPlot::RightLegend );
//	connect( m_legendPanel, &QwtLegend::checked,&ChartWave_qwt::showItem);
	connect( m_legendPanel, SIGNAL( checked( const QVariant &, bool, int ) ),
		SLOT( showItem( const QVariant &, bool ) ) );

	QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve );
	for ( int i = 0; i < items.size(); i++ )
	{
		const QVariant itemInfo = itemToInfo( items[i] );
		QwtLegendLabel *legendLabel =
			qobject_cast<QwtLegendLabel *>( m_legendPanel->legendWidget( itemInfo ) );
		if ( legendLabel )
			legendLabel->setChecked( items[i]->isVisible() );
	}
}
void SA2DGraph::deletelegendPanel()
{
	insertLegend( nullptr);//内部会销毁
	m_legendPanel = nullptr;
}

void SA2DGraph::setupYDataPicker()
{
    if (nullptr == m_yDataPicker)
	{
        m_yDataPicker = new SAYDataTracker( this->canvas() );
        m_yDataPicker->setRubberBandPen( QPen( "MediumOrchid" ) );
	}
}
void SA2DGraph::deleteYDataPicker()
{
    if (nullptr != m_yDataPicker)
	{
        m_yDataPicker->setParent(nullptr);//断开和canvas()的父子连接
        delete m_yDataPicker;
        m_yDataPicker = nullptr;
    }
}

void SA2DGraph::setupXYDataPicker()
{
    if (nullptr == m_xyDataPicker)
    {
        m_xyDataPicker = new SAXYDataTracker( this->canvas() );
        m_xyDataPicker->setRubberBandPen( QPen( "MediumOrchid" ) );
    }
}

void SA2DGraph::deleteXYDataPicker()
{
    if (nullptr != m_xyDataPicker)
    {
        m_xyDataPicker->setParent(nullptr);//断开和canvas()的父子连接
        delete m_xyDataPicker;
        m_xyDataPicker = nullptr;
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
    if(m_zoomer)
        return m_zoomer->isEnabled();
    if(m_zoomerSecond)
        return m_zoomerSecond->isEnabled();
    return false;
}
///
/// \brief 是否允许十字光标
/// \return
///
bool SA2DGraph::isEnablePicker() const
{
    if(m_picker)
        return m_picker->isEnabled();
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
QList<QwtPlotMarker*> SA2DGraph::getMakerList()
{
    QList<QwtPlotMarker*> list;
    QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotMarker);
    for(int i(0);i<items.size();++i){
        list.append(static_cast<QwtPlotMarker*>( items[i] ));
    }
    return list;
}

QwtPlotCurve* SA2DGraph::getCurveByTitle(const QString& strName)
{
    QList<QwtPlotCurve*> curs = getCurveList();
    QwtPlotCurve* cur(nullptr);
    for(int i(0);i<curs.size();++i){
        cur = curs[i];
        if(cur)
        {
            if(strName == cur->title().text())
                return cur;
        }
    }
    return nullptr;
}

void SA2DGraph::getYDatas(QVector<double>& ys,int nCur)
{
    QwtPlotCurve* cur = getCurveList().at(nCur);
    if(nullptr == cur){
        return;
    }
    SAChart::getYDatas(ys,cur);
}

void SA2DGraph::getYDatas(QVector<double>& ys,const QString& strCurName)
{
    QwtPlotCurve* cur = getCurveByTitle(strCurName);
    if(nullptr == cur){
        return;
    }
    SAChart::getYDatas(ys,cur);
}

void SA2DGraph::getXDatas(QVector<double>& xs,int nCur)
{
    QwtPlotCurve* cur = getCurveList().at(nCur);
    if(nullptr == cur){
        return;
    }
    SAChart::getXDatas(xs,cur);
}

void SA2DGraph::getXDatas(QVector<double>& xs,const QString& strCurName)
{
    QwtPlotCurve* cur = getCurveByTitle(strCurName);
    if(nullptr == cur){
        return;
    }
    SAChart::getXDatas(xs,cur);
}

void SA2DGraph::getXYDatas(QVector<QPointF>& xys,int nCur)
{
    QwtPlotCurve* cur = getCurveList().at(nCur);
    if(nullptr == cur){
        return;
    }
    SAChart::getXYDatas(xys,cur);

}

void SA2DGraph::getXYDatas(QVector<QPointF>& xys,const QString& strCurName)
{
    QwtPlotCurve* cur = getCurveByTitle(strCurName);
    if(nullptr == cur){
        return;
    }
    SAChart::getXYDatas(xys,cur);
}

void SA2DGraph::getXYDatas(QVector<double>& xs,QVector<double>& ys,int nCur)
{
    QwtPlotCurve* cur = getCurveList().at(nCur);
    if(nullptr == cur){
        return;
    }
    SAChart::getYDatas(ys,cur);
    SAChart::getXDatas(xs,cur);
}

void SA2DGraph::getXYDatas(QVector<double>& xs,QVector<double>& ys,const QString& strCurName)
{
    QwtPlotCurve* cur = getCurveByTitle(strCurName);
    if(nullptr == cur){
        return;
    }
    SAChart::getYDatas(ys,cur);
    SAChart::getXDatas(xs,cur);
}

///
/// \brief 获取当前显示区域的数据
/// \param out_xys
/// \param curve
/// \return 返回区域的索引x为第一个索引，y为第二个索引
///
QPoint SA2DGraph::getPlottingRegionDatas(QVector<QPointF>& out_xys,QwtPlotCurve* curve) const
{
	if (!curve)
		return QPoint(0,0);
	QPoint boundary(0,0);
    QwtPlot::Axis xaxis = QwtPlot::xBottom;
    if(!axisEnabled(QwtPlot::xBottom))
        xaxis = QwtPlot::xTop;
    QwtInterval xInter = axisInterval(xaxis);
	double min = xInter.minValue();
	double max = xInter.maxValue();

	auto pdatas = curve->data();
	size_t n = pdatas->size();
	out_xys.reserve(n);
	bool firstIn(true);
    for(size_t i=0;i<n;++i)
	{
		if(pdatas->sample(i).x()>=min 
			&&
			pdatas->sample(i).x()<= max)
		{
			out_xys.push_back(pdatas->sample(i));
			if (firstIn)
			{
				boundary.rx() = i;
				firstIn = false;
			}
			boundary.ry() = i;
		}
	}
	return boundary;
}

QPoint SA2DGraph::getPlottingRegionDatas(std::vector<double>& out_xs
	,std::vector<double>& out_ys
    ,QwtPlotCurve* curve) const
{
	if (!curve)
		return QPoint(0,0);
	QPoint boundary(0,0);
    QwtPlot::Axis xaxis = QwtPlot::xBottom;
    if(!axisEnabled(QwtPlot::xBottom))
        xaxis = QwtPlot::xTop;
    QwtInterval xInter = axisInterval(xaxis);
	double min = xInter.minValue();
	double max = xInter.maxValue();

	auto pdatas = curve->data();
	size_t n = pdatas->size();
	out_xs.reserve(n);
	out_ys.reserve(n);
	bool firstIn(true);
    for(size_t i=0;i<n;++i)
	{
		if(pdatas->sample(i).x()>=min 
			&&
			pdatas->sample(i).x()<= max)
		{
			out_xs.push_back(pdatas->sample(i).x());
			out_ys.push_back(pdatas->sample(i).y());
			if (firstIn)
			{
				boundary.rx() = i;
				firstIn = false;
			}
			boundary.ry() = i;
		}
	}
    return boundary;
}

QPoint SA2DGraph::getPlottingRegionDatas(QVector<double>& out_xs, QVector<double>& out_ys, QwtPlotCurve* curve) const
{
    if (!curve)
        return QPoint(0,0);
    QPoint boundary(0,0);
    QwtPlot::Axis xaxis = QwtPlot::xBottom;
    if(!axisEnabled(QwtPlot::xBottom))
        xaxis = QwtPlot::xTop;
    QwtInterval xInter = axisInterval(xaxis);
    double min = xInter.minValue();
    double max = xInter.maxValue();

    auto pdatas = curve->data();
    size_t n = pdatas->size();
    out_xs.reserve(n);
    out_ys.reserve(n);
    bool firstIn(true);
    for(size_t i=0;i<n;++i)
    {
        if(pdatas->sample(i).x()>=min
            &&
            pdatas->sample(i).x()<= max)
        {
            out_xs.push_back(pdatas->sample(i).x());
            out_ys.push_back(pdatas->sample(i).y());
            if (firstIn)
            {
                boundary.rx() = i;
                firstIn = false;
            }
            boundary.ry() = i;
        }
    }
    return boundary;
}
///
/// \brief 获取当前正在显示的区域
/// \return
///
QRectF SA2DGraph::getPlottingRegionRang() const
{
    QwtPlot::Axis xaxis = QwtPlot::xBottom;
    if(!axisEnabled(QwtPlot::xBottom))
        xaxis = QwtPlot::xTop;
    QwtInterval inter = axisInterval(xaxis);
    double xmin = inter.minValue();
    double xmax = inter.maxValue();
    QwtPlot::Axis yaxis = QwtPlot::yLeft;
    if(!axisEnabled(QwtPlot::yLeft))
        yaxis = QwtPlot::yRight;
    inter = axisInterval(yaxis);
    double ymin = inter.minValue();
    double ymax = inter.maxValue();
    return QRectF(xmin,ymin,xmax-xmin,ymax-ymin);
}

///
/// \brief 把AxisDateScaleType转换为字符
/// \param type 类型
/// \return
///
QString SA2DGraph::axisDateScaleType2String(AxisDateScaleType type)
{
	switch (type)
	{
    case SA2DGraph::h_m: return QString("h:m");
    case SA2DGraph::hh_mm: return QString("hh:mm");
    case SA2DGraph::h_m_s: return QString("h:m:s");
    case SA2DGraph::hh_mm_ss: return QString("hh:mm:ss");
    case SA2DGraph::yyyy_M_d: return QString("yyyy-M-d");
    case SA2DGraph::yyyy_M_d_h_m: return QString("yyyy-M-d h:m");
    case SA2DGraph::yyyy_M_d_h_m_s: return QString("yyyy-M-d h:m:s");
    case SA2DGraph::yyyy_MM_dd: return QString("yyyy-MM-dd");
    case SA2DGraph::yyyy_MM_dd_hh_mm: return QString("yyyy-MM-dd hh:mm");
    case SA2DGraph::yyyy_MM_dd_hh_mm_ss: return QString("yyyy-MM-dd hh:mm:ss");
	}
    return QString("yyyy-MM-dd hh:mm:ss");
}

QStringList SA2DGraph::axisDateScaleTypes2StringList()
{
    return QStringList()<<"h:m"<<"hh:mm"<<"h:m:s"<<"hh:mm:ss"
                       <<"yyyy-M-d"<<"yyyy-M-d h:m"<<"yyyy-M-d h:m:s"
                      <<"yyyy-MM-dd"<<"yyyy-MM-dd hh:mm"<<"yyyy-MM-dd hh:mm:ss";
}
///
/// \brief 设置坐标轴为时间坐标轴
/// \param axisID 轴的类型 
/// \param 时间显示的类型 
/// \param type 类型 
///
void SA2DGraph::setAxisDateTimeScale(AxisDateScaleType type,int axisID,QwtDate::IntervalType intType)
{
	QString strDateFormat = axisDateScaleType2String(type);
    setAxisDateTimeScale(strDateFormat,axisID,intType);
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
///
/// \brief 改变时间轴的时间显示格式，如果
/// \param axis
/// \param format
/// \param intType
///
void SA2DGraph::setAxisDateFormat(QwtPlot::Axis axis, AxisDateScaleType format, QwtDate::IntervalType intType)
{
    QwtDateScaleDraw* dateScale = dynamic_cast<QwtDateScaleDraw*>(axisScaleDraw(axis));
    if(!dateScale)
        return;
    setAxisDateTimeScale(axisDateScaleType2String(format),axis,intType);
}
