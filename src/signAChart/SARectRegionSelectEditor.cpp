#include "SARectRegionSelectEditor.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>

SARectRegionSelectEditor::SARectRegionSelectEditor(QwtPlot *parent):SAAbstractRegionSelectEditor(parent)
  ,m_isEnable(false)
  ,m_isStartDrawRegion(false)
  ,m_shapeItem(nullptr)
  ,m_tmpItem(nullptr)
  ,m_selectedRect(QRectF())
{
    setEnabled(true);
    connect(parent,&QwtPlot::itemAttached,this,&SARectRegionSelectEditor::onItemAttached);
}

SARectRegionSelectEditor::~SARectRegionSelectEditor()
{
    if(m_shapeItem)
    {
        m_shapeItem->detach();
        delete m_shapeItem;
        m_shapeItem = nullptr;
    }
}


bool SARectRegionSelectEditor::eventFilter(QObject *object, QEvent *event)
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
                if(mouseEvent)
                {
                    const bool accepted = pressed( mouseEvent->pos() );
                }
                break;
            }
            case QEvent::MouseMove:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast< QMouseEvent* >( event );
                if(mouseEvent)
                {
                    const bool accepted = moved( mouseEvent->pos() );
                }
                break;
            }
            case QEvent::MouseButtonRelease:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                if(mouseEvent)
                {
                    released( mouseEvent->pos() );
                }
                break;
            }
            case QEvent::KeyPress:
            {
                const QKeyEvent* keyEvent =
                    dynamic_cast<QKeyEvent* >( event );
                if(keyEvent)
                {
                    keyPressed(keyEvent);
                }
                break;
            }
            case QEvent::KeyRelease:
            {
                const QKeyEvent* keyEvent =
                    dynamic_cast<QKeyEvent* >( event );
                if(keyEvent)
                {
                    keyRelease(keyEvent);
                }
                break;
            }


            default:
                break;
        }
        return false;
    }
    return QObject::eventFilter( object, event );
}

bool SARectRegionSelectEditor::pressed(const QPoint &p)
{
    if(nullptr == m_shapeItem)
    {
        m_shapeItem = new SASelectRegionShapeItem("select region");
        m_shapeItem->attach(plot());
    }
    switch(getSelectionMode())
    {
    case SingleSelection://单一选择
    {
        m_isStartDrawRegion = true;
        m_pressedPoint = invTransform(p);
        break;
    }
    case AdditionalSelection:
    case SubtractionSelection:
    case IntersectionSelection:
    {
        if(nullptr == m_tmpItem)
        {
            m_tmpItem = new SASelectRegionShapeItem("temp region");
            m_tmpItem->attach(plot());
        }
        m_isStartDrawRegion = true;
        m_pressedPoint = invTransform(p);
        break;
    }
    default:
        break;
    }
    return true;
}

bool SARectRegionSelectEditor::moved(const QPoint &p)
{
    if(!m_isStartDrawRegion)
    {
        return true;
    }
    switch(getSelectionMode())
    {
    case SingleSelection:
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
        break;
    }
    case AdditionalSelection:
    case SubtractionSelection:
    case IntersectionSelection:
    {
        QPointF pf = invTransform(p);
        m_selectedRect.setX(m_pressedPoint.x());
        m_selectedRect.setY(m_pressedPoint.y());
        m_selectedRect.setWidth(pf.x() - m_pressedPoint.x());
        m_selectedRect.setHeight(pf.y() - m_pressedPoint.y());
        if(m_tmpItem)
        {
            m_tmpItem->setRect(m_selectedRect);
        }
        break;
    }
    }
    return true;
}

void SARectRegionSelectEditor::released(const QPoint &p)
{
    switch(getSelectionMode())
    {
    case SingleSelection:
    {
        if(m_isStartDrawRegion)
        {
            QPointF pf = invTransform(p);
            if(pf == m_pressedPoint)
            {
                //如果点击和松开是一个点，就取消当前的选区
                if(m_shapeItem)
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
        break;
    }
    case AdditionalSelection:
    case SubtractionSelection:
    case IntersectionSelection:
    {
        if(m_isStartDrawRegion)
        {
            QPointF pf = invTransform(p);
            if(pf == m_pressedPoint)
            {
                //如果点击和松开是一个点，就取消当前的选区
                if(m_tmpItem)
                {
                    m_tmpItem->detach();
                    delete m_tmpItem;
                    m_tmpItem = nullptr;
                }
                m_isStartDrawRegion = false;
                return;
            }
            m_selectedRect.setX(m_pressedPoint.x());
            m_selectedRect.setY(m_pressedPoint.y());
            m_selectedRect.setWidth(pf.x() - m_pressedPoint.x());
            m_selectedRect.setHeight(pf.y() - m_pressedPoint.y());
            if(m_shapeItem)
            {
                QPainterPath shape = m_shapeItem->shape();
                QPainterPath addtion;
                addtion.addRect(m_selectedRect);
                switch(getSelectionMode())
                {
                case AdditionalSelection:
                {
                    shape = shape.united(addtion);
                    break;
                }
                case SubtractionSelection:
                {
                    shape = shape.subtracted(addtion);
                    break;
                }
                case IntersectionSelection:
                {
                    shape = shape.intersected(addtion);
                    break;
                }
                default:
                    break;
                }
                m_shapeItem->setShape(shape);
            }
            if(m_tmpItem)
            {
                m_tmpItem->detach();
                delete m_tmpItem;
                m_tmpItem = nullptr;
            }
        }
        m_isStartDrawRegion = false;
        break;
    }
    }
}
///
/// \brief 处理按钮事件
/// \param e
///
void SARectRegionSelectEditor::keyPressed(const QKeyEvent *e)
{

}
void SARectRegionSelectEditor::keyRelease(const QKeyEvent *e)
{

}

void SARectRegionSelectEditor::setEnabled(bool on)
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

bool SARectRegionSelectEditor::isEnabled() const
{
    return m_isEnable;
}
///
/// \brief 判断当前的选择区域是否显示
/// \return
///
bool SARectRegionSelectEditor::isRegionVisible() const
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
/// \brief 获取选框区域的item
/// \return
///
const QwtPlotShapeItem *SARectRegionSelectEditor::getShapeItem() const
{
    return m_shapeItem;
}
///
/// \brief 获取选框区域的item
/// \return
///
QwtPlotShapeItem *SARectRegionSelectEditor::getShapeItem()
{
    return m_shapeItem;
}
///
/// \brief 获取选择的数据区域
/// \return
///
QPainterPath SARectRegionSelectEditor::getSelectRegion() const
{
    if(nullptr == m_shapeItem)
    {
        return QPainterPath();
    }
    return m_shapeItem->shape();
}

void SARectRegionSelectEditor::setSelectRegion(const QPainterPath &shape)
{
    if(nullptr == m_shapeItem)
    {
        m_shapeItem = new SASelectRegionShapeItem("select region");
        m_shapeItem->attach(plot());
    }
}
///
/// \brief 判断点是否在区域里
/// \param p
/// \return
///
bool SARectRegionSelectEditor::isContains(const QPointF &p) const
{
    return getSelectRegion().contains(p);
}
///
/// \brief 设置选择模式
/// \param selectionMode
///
void SARectRegionSelectEditor::setSelectionMode(const SARectRegionSelectEditor::SelectionMode &selectionMode)
{
    switch(selectionMode)
    {
    case SingleSelection:
    {
        if(m_tmpItem)
        {
            m_tmpItem->detach();
            delete m_tmpItem;
            m_tmpItem = nullptr;
        }
    }
    default:
        break;
    }

    SAAbstractRegionSelectEditor::setSelectionMode(selectionMode);
}

///
/// \brief 清理数据
///
void SARectRegionSelectEditor::clear()
{
    if(m_shapeItem)
    {
        m_shapeItem->detach();
        delete m_shapeItem;
        m_shapeItem = nullptr;
    }
    m_selectedRect = QRectF();
}

void SARectRegionSelectEditor::onItemAttached(QwtPlotItem *item, bool on)
{
    if(!on)
    {
        if(item == m_shapeItem)
        {
            m_shapeItem = nullptr;
        }
        if(item == m_tmpItem)
        {
            m_tmpItem = nullptr;
        }
    }
}


