#include "SARectSelectEditor.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
SARectSelectEditor::SARectSelectEditor(QwtPlot *parent):SAAbstractRegionSelectEditor(parent)
  ,m_isEnable(false)
  ,m_isStartDrawRegion(false)
  ,m_shapeItem(nullptr)
  ,m_selectedRect(QRectF())
  ,m_xAxis(QwtPlot::xBottom)
  ,m_yAxis(QwtPlot::yLeft)
{
    setEnabled(true);
    connect(parent,&QwtPlot::itemAttached,this,&SARectSelectEditor::onItemAttached);
}

SARectSelectEditor::~SARectSelectEditor()
{
    if(m_shapeItem)
    {
        m_shapeItem->detach();
        delete m_shapeItem;
        m_shapeItem = nullptr;
    }
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
        QPointF pf = invTransform(p);
        m_selectedRect.setX(m_pressedPoint.x());
        m_selectedRect.setY(m_pressedPoint.y());
        m_selectedRect.setWidth(pf.x() - m_pressedPoint.x());
        m_selectedRect.setHeight(pf.y() - m_pressedPoint.y());
        if(m_shapeItem)
        {
            m_shapeItem->setRect(m_selectedRect);
        }
    }
    return true;
}

void SARectSelectEditor::released(const QPoint &p)
{
    if(m_isStartDrawRegion)
    {
        QPointF pf = invTransform(p);
        if(pf == m_pressedPoint)
        {
            //如果点击和松开是一个点，就取消当前的选区
            m_shapeItem->setRect(QRectF());
            m_isStartDrawRegion = false;
            return;
        }
        m_selectedRect.setX(m_pressedPoint.x());
        m_selectedRect.setY(m_pressedPoint.y());
        m_selectedRect.setWidth(pf.x() - m_pressedPoint.x());
        m_selectedRect.setHeight(pf.y() - m_pressedPoint.y());
        if(m_shapeItem)
        {
            m_shapeItem->setRect(m_selectedRect);
        }
    }
    m_isStartDrawRegion = false;
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
///
/// \brief 判断当前的选择区域是否显示
/// \return
///
bool SARectSelectEditor::isRegionVisible() const
{
    if(m_shapeItem)
    {
        if(m_shapeItem->isVisible())
        {
            return true;
        }
    }
    return false;
}
///
/// \brief 屏幕坐标转换为数据坐标
/// \param pos
/// \return
///
QPointF SARectSelectEditor::invTransform( const QPoint &pos ) const
{
    QwtScaleMap xMap = plot()->canvasMap( m_xAxis );
    QwtScaleMap yMap = plot()->canvasMap( m_yAxis );

    return QPointF(
        xMap.invTransform( pos.x() ),
        yMap.invTransform( pos.y() )
                );
}
///
/// \brief 设置关联的坐标轴
/// \note 默认是xbottom，yLeft
/// \param xAxis
/// \param yAxis
///
void SARectSelectEditor::setAxis(int xAxis, int yAxis)
{
    m_xAxis = xAxis;
    m_yAxis = yAxis;
}
///
/// \brief 获取选框区域的item
/// \return
///
const QwtPlotShapeItem *SARectSelectEditor::getShapeItem() const
{
    return m_shapeItem;
}
///
/// \brief 获取选框区域的item
/// \return
///
QwtPlotShapeItem *SARectSelectEditor::getShapeItem()
{
    return m_shapeItem;
}
///
/// \brief 获取选择的数据区域
/// \return
///
QPainterPath SARectSelectEditor::getSelectRegion() const
{
    if(nullptr == m_shapeItem)
    {
        return QPainterPath();
    }
    return m_shapeItem->shape();
}
///
/// \brief 判断点是否在区域里
/// \param p
/// \return
///
bool SARectSelectEditor::isContains(const QPointF &p) const
{
    return getSelectRegion().contains(p);
}

///
/// \brief 清理数据
///
void SARectSelectEditor::clear()
{
    if(m_shapeItem)
    {
        m_shapeItem->detach();
        delete m_shapeItem;
        m_shapeItem = nullptr;
    }
    m_selectedRect = QRectF();
}

void SARectSelectEditor::onItemAttached(QwtPlotItem *item, bool on)
{
    if(!on)
    {
        if(item == m_shapeItem)
        {
            m_shapeItem = nullptr;
        }
    }
}
