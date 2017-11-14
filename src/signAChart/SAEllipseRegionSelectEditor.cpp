#include "SAEllipseRegionSelectEditor.h"
#include <QMouseEvent>
#include <QKeyEvent>

SAEllipseRegionSelectEditor::SAEllipseRegionSelectEditor(QwtPlot *parent)
    :SAAbstractRegionSelectEditor(parent)
    ,m_isStartDrawRegion(false)
    ,m_shapeItem(nullptr)
    ,m_tmpItem(nullptr)
    ,m_selectedRect(QRectF())
{
    setEnabled(true);
    connect(parent,&QwtPlot::itemAttached,this,&SAEllipseRegionSelectEditor::onItemAttached);
}

SAEllipseRegionSelectEditor::~SAEllipseRegionSelectEditor()
{
    if(m_shapeItem)
    {
        m_shapeItem->detach();
        delete m_shapeItem;
        m_shapeItem = nullptr;
    }
    if(m_tmpItem)
    {
        m_tmpItem->detach();
        delete m_tmpItem;
        m_tmpItem = nullptr;
    }
}


bool SAEllipseRegionSelectEditor::isRegionVisible() const
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


QPainterPath SAEllipseRegionSelectEditor::getSelectRegion() const
{
    if(nullptr == m_shapeItem)
    {
        return QPainterPath();
    }
    return m_shapeItem->shape();
}

void SAEllipseRegionSelectEditor::setSelectRegion(const QPainterPath &shape)
{
    if(nullptr == m_shapeItem)
    {
        m_shapeItem = new SASelectRegionShapeItem("select region");
        m_shapeItem->setShape(shape);
        m_shapeItem->attach(plot());
    }
}


void SAEllipseRegionSelectEditor::setSelectionMode(const SAAbstractRegionSelectEditor::SelectionMode &selectionMode)
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
/// \brief rtti
/// \return
///
int SAEllipseRegionSelectEditor::rtti() const
{
    return RTTIEllipseRegionSelectEditor;
}

const QwtPlotShapeItem *SAEllipseRegionSelectEditor::getShapeItem() const
{
    return m_shapeItem;
}

QwtPlotShapeItem *SAEllipseRegionSelectEditor::getShapeItem()
{
    return m_shapeItem;
}

void SAEllipseRegionSelectEditor::clear()
{
    if(m_shapeItem)
    {
        m_shapeItem->detach();
        delete m_shapeItem;
        m_shapeItem = nullptr;
    }
    if(m_tmpItem)
    {
        m_tmpItem->detach();
        delete m_tmpItem;
        m_tmpItem = nullptr;
    }
    m_selectedRect = QRectF();
}

void SAEllipseRegionSelectEditor::onItemAttached(QwtPlotItem *item, bool on)
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

bool SAEllipseRegionSelectEditor::mousePressEvent(const QMouseEvent *e)
{
    if(Qt::MidButton == e->button() || Qt::RightButton == e->button())
    {
        return false;
    }
    QPoint p = e->pos();
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
        if(m_tmpItem)
        {
            m_tmpItem->detach();
            delete m_tmpItem;
            m_tmpItem = nullptr;
        }
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
        return false;
    }
    return true;
}

bool SAEllipseRegionSelectEditor::mouseMovedEvent(const QMouseEvent *e)
{
    if(!m_isStartDrawRegion)
    {
        return false;
    }
    if(Qt::MidButton == e->button() || Qt::RightButton == e->button())
    {
        return false;
    }
    QPoint p = e->pos();
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
            m_shapeItem->setEllipse(m_selectedRect);
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
            m_tmpItem->setEllipse(m_selectedRect);
        }
        break;
    }
    default:
        return false;
    }
    return true;
}

bool SAEllipseRegionSelectEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    if(Qt::MidButton == e->button() || Qt::RightButton == e->button())
    {
        return false;
    }
    QPoint p = e->pos();
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
                return true;
            }
            m_selectedRect.setX(m_pressedPoint.x());
            m_selectedRect.setY(m_pressedPoint.y());
            m_selectedRect.setWidth(pf.x() - m_pressedPoint.x());
            m_selectedRect.setHeight(pf.y() - m_pressedPoint.y());
            if(m_shapeItem)
            {
                m_shapeItem->setEllipse(m_selectedRect);
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
                return true;
            }
            m_selectedRect.setX(m_pressedPoint.x());
            m_selectedRect.setY(m_pressedPoint.y());
            m_selectedRect.setWidth(pf.x() - m_pressedPoint.x());
            m_selectedRect.setHeight(pf.y() - m_pressedPoint.y());
            if(m_shapeItem)
            {
                QPainterPath shape = m_shapeItem->shape();
                QPainterPath addtion;
                addtion.addEllipse(m_selectedRect);
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
    default:
        return false;
    }
    return true;
}


