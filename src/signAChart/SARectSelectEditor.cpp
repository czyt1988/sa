#include "SARectSelectEditor.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
SARectSelectEditor::SARectSelectEditor(QwtPlot *parent):QObject(parent)
    ,m_isEnable(false)
  ,m_shapeItem(nullptr)
  ,m_isStartDrawRegion(false)
  ,m_xAxis(QwtPlot::xBottom)
  ,m_yAxis(QwtPlot::yLeft)
{
    setEnabled(true);
}

SARectSelectEditor::~SARectSelectEditor()
{
    if(m_shapeItem)
    {
        m_shapeItem->detach();
        delete m_shapeItem;
    }
}

const QwtPlot *SARectSelectEditor::plot() const
{
    return qobject_cast<const QwtPlot *>( parent() );
}

QwtPlot *SARectSelectEditor::plot()
{
    return qobject_cast<QwtPlot *>( parent() );
}

bool SARectSelectEditor::eventFilter(QObject *object, QEvent *event)
{
    QwtPlot *plot = qobject_cast<QwtPlot *>( parent() );
    if ( plot && object == plot->canvas() )
    {
        switch( event->type() )
        {
            case QEvent::MouseButtonPress:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                const bool accepted = pressed( mouseEvent->pos() );
                break;
            }
            case QEvent::MouseMove:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast< QMouseEvent* >( event );
                const bool accepted = moved( mouseEvent->pos() );
                break;
            }
            case QEvent::MouseButtonRelease:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                released( mouseEvent->pos() );
                break;
            }
            default:
                break;
        }
        return false;
    }
    return QObject::eventFilter( object, event );
}

bool SARectSelectEditor::pressed(const QPoint &p)
{
    qDebug() << "pressed:"<< p;
    m_isStartDrawRegion = true;
    if(nullptr == m_shapeItem)
    {
        m_shapeItem = new SASelectRegionShapeItem("select region");
        m_shapeItem->attach(plot());
    }
    m_pressedPoint = invTransform(p);
    return true;
}

bool SARectSelectEditor::moved(const QPoint &p)
{
    if(m_isStartDrawRegion)
    {
        QRectF r;
        QPointF pf = invTransform(p);
        r.setX(m_pressedPoint.x());
        r.setY(m_pressedPoint.y());
        r.setWidth(pf.x() - m_pressedPoint.x());
        r.setHeight(pf.y() - m_pressedPoint.y());
        if(m_shapeItem)
        {
            m_shapeItem->setRect(r);
        }
    }
    return true;
}

void SARectSelectEditor::released(const QPoint &p)
{
qDebug() << "released:"<< p;
    if(m_isStartDrawRegion)
    {
        QRectF r;
        QPointF pf = invTransform(p);
        r.setX(m_pressedPoint.x());
        r.setY(m_pressedPoint.y());
        r.setWidth(pf.x() - m_pressedPoint.x());
        r.setHeight(pf.y() - m_pressedPoint.y());
        if(m_shapeItem)
        {
            m_shapeItem->setRect(r);
        }
    }
    m_isStartDrawRegion = false;
}

SARectSelectEditor::SelectionMode SARectSelectEditor::selectionMode() const
{
    return m_selectionMode;
}

void SARectSelectEditor::setSelectionMode(const SelectionMode &selectionMode)
{
    m_selectionMode = selectionMode;
}

void SARectSelectEditor::setEnabled(bool on)
{
    if ( on == m_isEnable )
        return;

    QwtPlot *plot = qobject_cast<QwtPlot *>( parent() );
    if ( plot )
    {
        m_isEnable = on;

        if ( on )
        {
            plot->canvas()->installEventFilter( this );
        }
        else
        {
            plot->canvas()->removeEventFilter( this );
        }
    }
}

bool SARectSelectEditor::isEnabled() const
{
    return m_isEnable;
}

QPointF SARectSelectEditor::invTransform( const QPoint &pos ) const
{
    QwtScaleMap xMap = plot()->canvasMap( m_xAxis );
    QwtScaleMap yMap = plot()->canvasMap( m_yAxis );

    return QPointF(
        xMap.invTransform( pos.x() ),
        yMap.invTransform( pos.y() )
                );
}

void SARectSelectEditor::setAxis(int xAxis, int yAxis)
{
    m_xAxis = xAxis;
    m_yAxis = yAxis;
}
