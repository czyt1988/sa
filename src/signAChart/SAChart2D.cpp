#include "SAChart2D.h"
#include <qstyle.h>
#include <qstyleoption.h>

#include <qwt_interval.h>
#include <qwt_picker_machine.h>
#include <algorithm>
#include <qwt_legend_label.h>
#include <qwt_date_scale_draw.h>

#include <SAPlotMarker.h>
#include <SAYDataTracker.h>
#include <SAXYDataTracker.h>
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

class Picker_qwt: public QwtPlotPicker
{
public:
	Picker_qwt(int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QWidget *parent)
		:QwtPlotPicker(xAxis,yAxis,rubberBand,trackerMode,parent)
	{}
	Picker_qwt(QWidget *canvas):QwtPlotPicker(canvas)
	{
		setTrackerMode( QwtPlotPicker::AlwaysOn );//这是指定文字的显示，AlwaysOn值，光标不激活，也显示文字提示
		setRubberBand( QwtPlotPicker::CrossRubberBand );
		setStateMachine(  new QwtPickerTrackerMachine() );//QwtPickerTrackerMachine是不用鼠标激活
		//如果是new QwtPickerDragPointMachine()就是鼠标点击激活
	}
	~Picker_qwt(){}
	virtual QwtText trackerTextF( const QPointF &pos ) const
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

SAChart2D::SAChart2D(QWidget *parent):QwtPlot(parent)
  ,m_grid(nullptr)
  ,m_picker(nullptr)
  ,m_zoomer(nullptr)
  ,m_zoomerSecond(nullptr)
  ,m_panner(nullptr)
  ,m_legend(nullptr)
  ,m_legendPanel(nullptr)
  ,m_yDataPicker(nullptr)
  ,m_xyDataPicker(nullptr)
  ,m_bEnableZoom(false)
  ,m_bEnableCrosserPicker(false)
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
}

SAChart2D::~SAChart2D()
{

}

void SAChart2D::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
	
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11
    //updateGradient();
}

bool SAChart2D::isEnableZoomerScroll() const
{
    Zoomer_qwt* zm = qobject_cast<Zoomer_qwt*>(m_zoomer);
    if(zm)
    {
        return zm->isEnableScrollBar();
    }
    return false;
}

bool SAChart2D::isEnableGrid() const
{
    if(m_grid)
        return m_grid->isVisible();
    return false;
}

bool SAChart2D::isEnableGridX() const
{
    if(m_grid)
        if(m_grid->isVisible())
            return m_grid->xEnabled();
    return false;
}

bool SAChart2D::isEnableGridY() const
{
    if(m_grid)
        if(m_grid->isVisible())
            return m_grid->yEnabled();
    return false;
}

bool SAChart2D::isEnableGridXMin() const
{
    if(m_grid)
        if(m_grid->isVisible())
            return m_grid->xMinEnabled();
    return false;
}

bool SAChart2D::isEnableGridYMin() const
{
    if(m_grid)
        if(m_grid->isVisible())
            return m_grid->yMinEnabled();
    return false;
}

bool SAChart2D::isEnablePanner() const
{
    if(m_panner)
        return m_panner->isEnabled();
    return false;
}

bool SAChart2D::isEnableLegend() const
{
    if(m_legend)
        return m_legend->isVisible();
    return false;
}

bool SAChart2D::isEnableLegendPanel() const
{
    if(m_legendPanel)
        return m_legendPanel->isVisible();
    return false;

}

bool SAChart2D::isEnableYDataPicker() const
{
    if(m_yDataPicker)
        return m_yDataPicker->isEnabled();
    return false;
}

bool SAChart2D::isEnableXYDataPicker() const
{
    if(m_xyDataPicker)
        return m_xyDataPicker->isEnabled();
    return false;
}
//========================================================================================
//网格 grid 操作
//========================================================================================
QwtPlotGrid* SAChart2D::setupGrid(
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

void SAChart2D::deleteGrid(){
    if(nullptr == m_grid){
        return;
    }
    m_grid->detach();
    delete m_grid;
    m_grid = nullptr;
    replot();//刷新，否则不显示
}

void SAChart2D::enableGrid(bool isShow)
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

void SAChart2D::enableGridX(bool enable)
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

void SAChart2D::enableGridY(bool enable)
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

void SAChart2D::enableGridXMin(bool enable)
{
	if (nullptr == m_grid)
		return;
	m_grid->enableXMin(enable);
	emit enableGridXMinChanged(enable);
}
void SAChart2D::enableGridYMin(bool enable)
{
	if (nullptr == m_grid)
		return;
	m_grid->enableYMin(enable);
	emit enableGridYMinChanged(enable);
}

//========================================================================================
//画线和数据 操作
//========================================================================================

QwtPlotCurve* SAChart2D::addCurve(const QVector<QPointF>& xyDatas)
{
    QwtPlotCurve* pCurve = nullptr;
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(xyDatas);
    pCurve->attach(this);
    return pCurve;
}

QwtPlotCurve* SAChart2D::addCurve(std::vector<std::pair<double,double>>& xyDatas)
{
    QwtPlotCurve* pCurve = nullptr;
    QVector<double> x,y;
    x.reserve(xyDatas.size());
    y.reserve(xyDatas.size());
    std::vector<std::pair<double,double>>::iterator ite;
    for(ite = xyDatas.begin();ite != xyDatas.end();++ite){
        x.push_back(ite->first);
        y.push_back(ite->second);
    }
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
  //  pCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(x,y);
    pCurve->attach(this);

    return pCurve;
}
QwtPlotCurve* SAChart2D::addCurve(std::vector<double>& xDatas,std::vector<double>& yDatas)
{
    QwtPlotCurve* pCurve = nullptr;
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(&xDatas[0],&yDatas[0],xDatas.size() <= yDatas.size() ? xDatas.size() : yDatas.size());
    pCurve->attach(this);
    return pCurve;
}

QwtPlotCurve* SAChart2D::addCurve(const double *xData, const double *yData, int size)
{
    QwtPlotCurve* pCurve = nullptr;
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(xData,yData,size);
    pCurve->attach(this);
    return pCurve;
}

QwtPlotCurve* SAChart2D::addCurve(const QVector< double > &xData
                                      , const QVector< double > &yData)
{
    QwtPlotCurve* pCurve = nullptr;
    pCurve = new QwtPlotCurve;
    pCurve->setYAxis(yLeft);
    pCurve->setXAxis(xBottom);
    pCurve->setStyle(QwtPlotCurve::Lines);
    pCurve->setSamples(xData,yData);
    pCurve->attach(this);
    return pCurve;
}

void SAChart2D::setCurveSymbol(QwtPlotCurve *cur, QwtSymbol::Style style, const QSize &size)
{
    QBrush brush = cur->brush();
    QPen pen = cur->pen();
    QwtSymbol* symbol = new QwtSymbol(style,brush,pen,size);
    cur->setSymbol(symbol);
}

void SAChart2D::setCurveLinePenStyle(QwtPlotCurve *cur, Qt::PenStyle style)
{
    QPen pen = cur->pen();
    pen.setStyle(style);
    cur->setPen(pen);
}

void SAChart2D::addCurve(QwtPlotCurve* pC)
{
    pC->attach(this);
}
///
/// \brief 添加一条竖直线
/// \return
///
QwtPlotMarker* SAChart2D::addVLine(double val)
{
    QwtPlotMarker *marker = new QwtPlotMarker();
    marker->setXValue(val);
    marker->setLineStyle( QwtPlotMarker::VLine );
    marker->setItemAttribute( QwtPlotItem::Legend, true );
    marker->attach( this );
    return marker;
}
QwtPlotMarker* SAChart2D::addHLine(double val)
{
    QwtPlotMarker *marker = new QwtPlotMarker();
    marker->setYValue(val);
    marker->setLineStyle( QwtPlotMarker::HLine );
    marker->setItemAttribute( QwtPlotItem::Legend, true );
    marker->attach( this );
    return marker;
}

QwtPlotHistogram*SAChart2D::addHistogram(const QVector<QwtIntervalSample>& sample)
{
    QwtPlotHistogram* his = new QwtPlotHistogram();
    his->setSamples (sample);
    his->setYAxis(yLeft);
    his->setXAxis(xBottom);
    his->setStyle (QwtPlotHistogram::Columns);
    his->attach (this);
    return his;
}

///
/// \brief 在图片上标记点
/// \param pos 点的位置
/// \param strLabel 说明
/// \param type 样式，默认为0，就是箭头
/// \todo type and clr show be use
///
void SAChart2D::markPoint(QPointF pos, const QString& strLabel, QColor clr, int type)
{
    SAPointMarker* pointMark = new SAPointMarker(pos);
    SATriangleMarkSymbol* tra = new SATriangleMarkSymbol();
    pointMark->setSymbol( tra );
    pointMark->setLabel(strLabel);
    pointMark->setLabelAlignment(Qt::AlignTop|Qt::AlignHCenter);
    pointMark->setSpacing(10);//设置文字和mark的间隔
    pointMark->attach( this );


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
void SAChart2D::markYValue(double data, const QString &strLabel, QColor clr, int type)
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
void SAChart2D::removeAllMarker()
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
void SAChart2D::deleteItems(const QList<int> markRtti)
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

double SAChart2D::axisXmin(int axisId) const
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

double SAChart2D::axisXmax(int axisId) const
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

double SAChart2D::axisYmin(int axisId) const
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

double SAChart2D::axisYmax(int axisId) const
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


void SAChart2D::setupPicker()
{
    if(nullptr == m_picker)
    {
        //m_picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft,
        //    QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,canvas() );
// 		m_picker = new Picker_qwt( QwtPlot::xBottom, QwtPlot::yLeft,
//             QwtPlotPicker::CrossRubberBand,QwtPlotPicker::ActiveOnly /*QwtPicker::AlwaysOn*/,canvas() );
		m_picker = new Picker_qwt( this->canvas() );
        m_picker->setRubberBandPen(  QPen( QColor(186,85,211) ) );//QPen( "MediumOrchid" )

    }

}

void SAChart2D::enablePicker(bool enable)
{
    if(nullptr == m_picker)
        setupPicker();
	m_picker->setEnabled( enable );
    if(nullptr != m_zoomer)
    {
        if(enable && isEnableZoomer())
            m_zoomer->setTrackerMode( QwtPicker::AlwaysOff );
        if(!enable && isEnableZoomer())
            m_zoomer->setTrackerMode( QwtPicker::AlwaysOn );
    }
    m_bEnableCrosserPicker = enable;
	emit enablePickerChanged(enable);
}


void SAChart2D::enablePanner(bool enable)
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
void SAChart2D::setupPanner()
{
	//设置拖动
	if (nullptr == m_panner)
	{
		m_panner = new QwtPlotPanner( canvas() );
		m_panner->setMouseButton( Qt::MidButton );
	}
}
void SAChart2D::deletePanner()
{
	if (nullptr != m_panner)
	{
		m_panner->setParent(nullptr);//断开和canvas()的父子连接
		delete m_panner;
		m_panner = nullptr;
	}
}

void SAChart2D::enableZoomer(bool enable)
{
    if(nullptr == m_zoomer /*|| nullptr == m_zoomerSecond*/)
        setupZoomer(false);
    if(enable)
    {
        m_zoomer->setZoomBase( false );
        m_zoomer->setEnabled(enable);

//        m_zoomerSecond->setZoomBase( false );
//        m_zoomerSecond->setEnabled(enable);

        if(enable && isEnablePicker())//如果十指光标激活了，就关闭坐标提示
            m_zoomer->setTrackerMode( QwtPicker::AlwaysOff );
        if(enable && !isEnablePicker())
            m_zoomer->setTrackerMode( QwtPicker::AlwaysOn );
        //m_zoomer->zoom( 0 );
    }
    else
    {
        m_zoomer->setEnabled(false);
//        m_zoomerSecond->setEnabled(false);
        //deleteZoomer();
    }
    m_bEnableZoom = enable;
    emit enableZoomerChanged(enable);
}
void SAChart2D::setupZoomer(bool isHaveScroll)
{
    if(nullptr == m_zoomer)
    {
        qDebug()<<"setup zoom";
        Zoomer_qwt* zoom = new Zoomer_qwt(xBottom,yLeft, canvas());//Zoomer_qwt( QwtPlot::xBottom, QwtPlot::yLeft,canvas() );

        zoom->on_enable_scrollBar(isHaveScroll);
        zoom->setRubberBand( QwtPicker::RectRubberBand );
        zoom->setTrackerPen( QColor( Qt::black ) );

        zoom->setMousePattern( QwtEventPattern::MouseSelect2,
                Qt::RightButton, Qt::ControlModifier );
        zoom->setMousePattern( QwtEventPattern::MouseSelect3,
                Qt::RightButton );
        zoom->setTrackerMode( QwtPicker::AlwaysOff );
        m_zoomer = zoom;
    }
    if(nullptr == m_zoomerSecond)
    {
//        Zoomer_qwt* zoom = new Zoomer_qwt(xTop,yRight, canvas());//Zoomer_qwt( QwtPlot::xBottom, QwtPlot::yLeft,canvas() );
//        zoom->on_enable_scrollBar(isHaveScroll);
//        zoom->setRubberBand( QwtPicker::RectRubberBand );

//        zoom->setTrackerMode( QwtPicker::AlwaysOff );

//        zoom->setMousePattern( QwtEventPattern::MouseSelect2,
//                Qt::RightButton, Qt::ControlModifier );
//        zoom->setMousePattern( QwtEventPattern::MouseSelect3,
//                Qt::RightButton );
//        m_zoomerSecond = zoom;
    }
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );
    magnifier->setMouseButton( Qt::NoButton );

}

void SAChart2D::deleteZoomer()
{
    if(m_zoomer)
    {
        delete m_zoomer;
        m_zoomer = nullptr;
    }
    if(m_zoomerSecond)
    {
        delete m_zoomerSecond;
        m_zoomerSecond = nullptr;
    }
}
///
/// \brief 设置是否显示滚动条
/// \param enable
///
void SAChart2D::enableZoomerScroll(bool enable)
{
    Zoomer_qwt* zm = qobject_cast<Zoomer_qwt*>(m_zoomer);
    if(zm)
    {
        zm->on_enable_scrollBar(enable);
    }
}
///
/// \brief 设置缩放重置
///
void SAChart2D::setZoomReset()
{
    setAxisAutoScale(QwtPlot::yLeft,true);
    setAxisAutoScale(QwtPlot::xBottom,true);
    replot();
    if(m_zoomer)
    {
        m_zoomer->setZoomBase(false);

    }
    if(m_zoomerSecond)
    {
        m_zoomerSecond->setZoomBase(false);
    }
    if(m_zoomer)
        m_zoomer->zoom(0);
//    qDebug()<<"zoomBase1:"<<m_zoomer->zoomBase()<<",zoomBase2:"<<m_zoomerSecond->zoomBase();
}

void SAChart2D::setupLegend()
{
	if (nullptr == m_legend)
	{
		m_legend = new LegendItem();
		m_legend->attach( this );
	}
}

void SAChart2D::enableLegend(bool enable)
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

void SAChart2D::enableLegendPanel(bool enable )
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
void SAChart2D::setuplegendPanel()
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
void SAChart2D::deletelegendPanel()
{
	insertLegend( nullptr);//内部会销毁
	m_legendPanel = nullptr;
}

void SAChart2D::setupYDataPicker()
{
    if (nullptr == m_yDataPicker)
	{
        m_yDataPicker = new SAYDataTracker( this->canvas() );
        m_yDataPicker->setRubberBandPen( QPen( "MediumOrchid" ) );
	}
}
void SAChart2D::deleteYDataPicker()
{
    if (nullptr != m_yDataPicker)
	{
        m_yDataPicker->setParent(nullptr);//断开和canvas()的父子连接
        delete m_yDataPicker;
        m_yDataPicker = nullptr;
    }
}

void SAChart2D::setupXYDataPicker()
{
    if (nullptr == m_xyDataPicker)
    {
        m_xyDataPicker = new SAXYDataTracker( this->canvas() );
        m_xyDataPicker->setRubberBandPen( QPen( "MediumOrchid" ) );
    }
}

void SAChart2D::deleteXYDataPicker()
{
    if (nullptr != m_xyDataPicker)
    {
        m_xyDataPicker->setParent(nullptr);//断开和canvas()的父子连接
        delete m_xyDataPicker;
        m_xyDataPicker = nullptr;
    }
}

void SAChart2D::enableYDataPicker(bool enable)
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

void SAChart2D::enableXYDataPicker(bool enable)
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
void SAChart2D::showItem( const QVariant &itemInfo, bool on )
{
	QwtPlotItem *plotItem = infoToItem( itemInfo );
	if ( plotItem )
		plotItem->setVisible( on );
}

QList<QwtPlotCurve*> SAChart2D::getCurveList()
{
    QList<QwtPlotCurve*> curves;
    QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotCurve);
    for(int i(0);i<items.size();++i){
        curves.append(static_cast<QwtPlotCurve*>( items[i] ));
    }
    return curves;
}
QList<QwtPlotMarker*> SAChart2D::getMakerList()
{
    QList<QwtPlotMarker*> list;
    QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotMarker);
    for(int i(0);i<items.size();++i){
        list.append(static_cast<QwtPlotMarker*>( items[i] ));
    }
    return list;
}

QwtPlotCurve* SAChart2D::getCurveByTitle(const QString& strName)
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

void SAChart2D::getYDatas(QVector<double>& ys,int nCur)
{
    QwtPlotCurve* cur = getCurveList().at(nCur);
    if(nullptr == cur){
        return;
    }
    getYDatas(ys,cur);
//    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
//    if(nullptr == datas){
//        return;
//    }
//    ys = datas->yData();
}

void SAChart2D::getYDatas(QVector<double>& ys,const QString& strCurName)
{
    QwtPlotCurve* cur = getCurveByTitle(strCurName);
    if(nullptr == cur){
        return;
    }
    getYDatas(ys,cur);
//    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
//    if(nullptr == datas){
//        return;
//    }
//    ys = datas->yData();
}

void SAChart2D::getXDatas(QVector<double>& xs,int nCur)
{
    QwtPlotCurve* cur = getCurveList().at(nCur);
    if(nullptr == cur){
        return;
    }
    getXDatas(xs,cur);
//    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
//    if(nullptr == datas){
//        return;
//    }
//    xs = datas->xData();
}

void SAChart2D::getXDatas(QVector<double>& xs,const QString& strCurName)
{
    QwtPlotCurve* cur = getCurveByTitle(strCurName);
    if(nullptr == cur){
        return;
    }
    getXDatas(xs,cur);
//    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
//    if(nullptr == datas){
//        return;
//    }
//    xs = datas->xData();
}

void SAChart2D::getXYDatas(QVector<QPointF>& xys,int nCur)
{
    QwtPlotCurve* cur = getCurveList().at(nCur);
    if(nullptr == cur){
        return;
    }
    getXYDatas(xys,cur);
//    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
//    if(nullptr == datas){
//        return;
//    }
//    xys.reserve(datas->size());
//    for(unsigned int i=0;i< datas->size();++i)
//    {
//        xys.append(datas->sample(i));
//    }
}

void SAChart2D::getXYDatas(QVector<QPointF>& xys,const QString& strCurName)
{
    QwtPlotCurve* cur = getCurveByTitle(strCurName);
    if(nullptr == cur){
        return;
    }
    getXYDatas(xys,cur);
//    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
//    if(nullptr == datas){
//        return;
//    }
//    xys.reserve(datas->size());
//    for(unsigned int i=0;i< datas->size();++i)
//    {
//        xys.append(datas->sample(i));
//    }
}

void SAChart2D::getXYDatas(QVector<double>& xs,QVector<double>& ys,int nCur)
{
    QwtPlotCurve* cur = getCurveList().at(nCur);
    if(nullptr == cur){
        return;
    }
    getYDatas(ys,cur);
    getXDatas(xs,cur);
//    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
//    if(nullptr == datas){
//        return;
//    }
//    xs = datas->xData();
//    ys = datas->yData();
}

void SAChart2D::getXYDatas(QVector<double>& xs,QVector<double>& ys,const QString& strCurName)
{
    QwtPlotCurve* cur = getCurveByTitle(strCurName);
    if(nullptr == cur){
        return;
    }
    getYDatas(ys,cur);
    getXDatas(xs,cur);
//    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
//    if(nullptr == datas){
//        return;
//    }
//    xs = datas->xData();
    //    ys = datas->yData();
}
///
/// \brief 获取vector point的y
/// \param xys vector point
/// \param ys vector double y
/// \return 获取的尺寸
///
size_t SAChart2D::getYDatas(const QVector<QPointF>& xys, QVector<double>& ys)
{
    auto e = xys.end();
    auto s = 0;
    for(auto i=xys.begin();i!=e;++i,++s)
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
size_t SAChart2D::getXDatas(const QVector<QPointF>& xys, QVector<double>& xs)
{
    auto e = xys.end();
    auto s = 0;
    for(auto i=xys.begin();i!=e;++i,++s)
    {
        xs.push_back((*i).x());
    }
    return s;
}

size_t SAChart2D::getYDatas(QVector<double>& ys, QwtPlotCurve* cur, const QRectF &rang)
{
    QwtSeriesData<QPointF>* datas = cur->data();
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

size_t SAChart2D::getXDatas(QVector<double>& xs, QwtPlotCurve* cur, const QRectF &rang)
{
    QwtSeriesData<QPointF>* datas = cur->data();
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

size_t SAChart2D::getXYDatas(QVector<QPointF>& xys,const QwtPlotCurve* cur,const QRectF& rang)
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

size_t SAChart2D::getXYDatas(QVector<double>& xs, QVector<double>& ys, const QwtPlotCurve* cur, const QRectF& rang)
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

size_t SAChart2D::getXYDatas(QVector<QPointF>& xys, QVector<double>& xs, QVector<double>& ys, const QwtPlotCurve* cur, const QRectF& rang)
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

void SAChart2D::getSharpPeakPoint(QVector<QPointF>& sharpPoints,QwtPlotCurve* cur,bool getMax)
{
    QwtPointArrayData* datas = static_cast<QwtPointArrayData*>(cur->data());
    if(nullptr == datas)
        return;
    QVector<QPointF> points;
    points.reserve(datas->size());
    for(unsigned int i(0);i<datas->size();++i)
    {
        points.append(datas->sample(i));
    }
    getSharpPeakPoint(sharpPoints,points,getMax);
}

void SAChart2D::getSharpPeakPoint(QVector<QPointF>& sharpPoints,const QVector<QPointF>& Points,bool getMax)
{
    sharpPoints.clear();
    sharpPoints.reserve(int(Points.size()/2));
    int maxLoop = Points.size()-1;

    if(getMax)
    {
        for(int i=1;i<maxLoop;++i)
        {
            if((Points[i].y() > Points[i-1].y()) && (Points[i].y() > Points[i+1].y()))
            {
                sharpPoints.append(Points[i]);
            }
        }
    }
    else
    {
        for(int i=1;i<maxLoop;++i)
        {
            if((Points[i].y() < Points[i-1].y()) && (Points[i].y() < Points[i+1].y()))
            {
                sharpPoints.append(Points[i]);
            }
        }
    }
}


void SAChart2D::sort_sharpPeak(QVector<QPointF>& sharpPointsSorted,QwtPlotCurve* cur,bool getMax )
{
    getSharpPeakPoint(sharpPointsSorted,cur,getMax);
    std::sort(sharpPointsSorted.begin(),sharpPointsSorted.end(),cmpPointF_Y);
    std::reverse(sharpPointsSorted.begin(),sharpPointsSorted.end());//最大值需要进行一次翻转
}

void SAChart2D::sort_sharpPeak(QVector<QPointF>& sharpPointsSorted,const QVector<QPointF>& Points,bool getMax )
{
    getSharpPeakPoint(sharpPointsSorted,Points,getMax);
    std::sort(sharpPointsSorted.begin(),sharpPointsSorted.end(),cmpPointF_Y);
}
///
/// \brief 获取当前显示区域的数据
/// \param out_xys
/// \param curve
/// \return 返回区域的索引x为第一个索引，y为第二个索引
///
QPoint SAChart2D::getPlottingRegionDatas(QVector<QPointF>& out_xys,QwtPlotCurve* curve) const
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

QPoint SAChart2D::getPlottingRegionDatas(std::vector<double>& out_xs
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

QPoint SAChart2D::getPlottingRegionDatas(QVector<double>& out_xs, QVector<double>& out_ys, QwtPlotCurve* curve) const
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
QRectF SAChart2D::getPlottingRegionRang() const
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
/// \brief 把范围内的数据移除
/// \param removeRang 需要移除的数据范围
/// \param curve 需要移除数据的曲线
/// \return
///
size_t SAChart2D::removeDataInRang(const QRectF& removeRang,QwtPlotCurve* curve)
{
    //由于vector不善于删除，因此此处为了提高效率，先进行一份拷贝，再进行替换
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
///
/// \brief 把AxisDateScaleType转换为字符
/// \param type 类型
/// \return
///
QString SAChart2D::axisDateScaleType2String(AxisDateScaleType type)
{
	switch (type)
	{
    case SAChart2D::h_m: return QString("h:m");
    case SAChart2D::hh_mm: return QString("hh:mm");
    case SAChart2D::h_m_s: return QString("h:m:s");
    case SAChart2D::hh_mm_ss: return QString("hh:mm:ss");
    case SAChart2D::yyyy_M_d: return QString("yyyy-M-d");
    case SAChart2D::yyyy_M_d_h_m: return QString("yyyy-M-d h:m");
    case SAChart2D::yyyy_M_d_h_m_s: return QString("yyyy-M-d h:m:s");
    case SAChart2D::yyyy_MM_dd: return QString("yyyy-MM-dd");
    case SAChart2D::yyyy_MM_dd_hh_mm: return QString("yyyy-MM-dd hh:mm");
    case SAChart2D::yyyy_MM_dd_hh_mm_ss: return QString("yyyy-MM-dd hh:mm:ss");
	}
	return QString("yyyy-MM-dd hh:mm:ss");
}
///
/// \brief 设置坐标轴为时间坐标轴
/// \param axisID 轴的类型 
/// \param 时间显示的类型 
/// \param type 类型 
///
void SAChart2D::setDateAxis(AxisDateScaleType type,int axisID,QwtDate::IntervalType intType)
{
	QString strDateFormat = axisDateScaleType2String(type);
	setDateAxis(strDateFormat,axisID,intType);
}

void SAChart2D::setDateAxis(QString type,int axisID ,QwtDate::IntervalType intType)
{
    QwtDateScaleDraw* dateScale;
	dateScale = new QwtDateScaleDraw;
	dateScale->setDateFormat(intType,type);
    setAxisScaleDraw(axisID,dateScale);
}
///
/// \brief 改变时间轴的时间显示格式，如果
/// \param axis
/// \param format
/// \param intType
///
void SAChart2D::setAxisDateFormat(QwtPlot::Axis axis, AxisDateScaleType format, QwtDate::IntervalType intType)
{
    QwtDateScaleDraw* dateScale = dynamic_cast<QwtDateScaleDraw*>(axisScaleDraw(axis));
    if(!dateScale)
        return;
    setDateAxis(axisDateScaleType2String(format),axis,intType);
}
