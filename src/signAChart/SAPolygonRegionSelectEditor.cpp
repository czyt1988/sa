#include "SAPolygonRegionSelectEditor.h"
#include <QMouseEvent>
#include <QKeyEvent>

SAPolygonRegionSelectEditor::SAPolygonRegionSelectEditor(QwtPlot* parent)
    :SAAbstractRegionSelectEditor(parent)
    ,m_isStartDrawRegion(false)
    ,m_shapeItem(nullptr)
    ,m_tmpItem(nullptr)
    ,m_isFinishOneRegion(false)
{
    setEnabled(true);
    connect(parent,&QwtPlot::itemAttached,this,&SAPolygonRegionSelectEditor::onItemAttached);
}

SAPolygonRegionSelectEditor::~SAPolygonRegionSelectEditor()
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

bool SAPolygonRegionSelectEditor::isRegionVisible() const
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

QPainterPath SAPolygonRegionSelectEditor::getSelectRegion() const
{
    if(nullptr == m_shapeItem)
    {
        return QPainterPath();
    }
    return m_shapeItem->shape();
}

void SAPolygonRegionSelectEditor::setSelectRegion(const QPainterPath &shape)
{
    if(nullptr == m_shapeItem)
    {
        m_shapeItem = new SASelectRegionShapeItem("select region");
        m_shapeItem->setShape(shape);
        m_shapeItem->attach(plot());
    }
}

void SAPolygonRegionSelectEditor::setSelectionMode(const SAAbstractRegionSelectEditor::SelectionMode &selectionMode)
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

int SAPolygonRegionSelectEditor::rtti() const
{
    return RTTIPolygonRegionSelectEditor;
}

const QwtPlotShapeItem *SAPolygonRegionSelectEditor::getShapeItem() const
{
    return m_shapeItem;
}

QwtPlotShapeItem *SAPolygonRegionSelectEditor::getShapeItem()
{
    return m_shapeItem;
}

void SAPolygonRegionSelectEditor::onItemAttached(QwtPlotItem *item, bool on)
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

bool SAPolygonRegionSelectEditor::mousePressEvent(const QMouseEvent *e)
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
        m_polygon.append(invTransform(p));
        if(m_shapeItem)
        {
            m_shapeItem->setPolygon(m_polygon);
        }
        break;
    }
    case AdditionalSelection:
    case SubtractionSelection:
    case IntersectionSelection:
    {
        m_isStartDrawRegion = true;
        m_polygon.append(invTransform(p));

        if(nullptr == m_tmpItem)
        {
            m_tmpItem = new SASelectRegionShapeItem("temp region");
            m_tmpItem->attach(plot());
        }
        m_tmpItem->setPolygon(m_polygon);

        break;
    }
    default:
        return false;
    }
    return true;
}

bool SAPolygonRegionSelectEditor::mouseMovedEvent(const QMouseEvent *e)
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
        QPolygonF tmp = m_polygon;
        tmp.append(pf);
        if(m_shapeItem)
        {
            m_shapeItem->setPolygon(tmp);
        }
        break;
    }
    case AdditionalSelection:
    case SubtractionSelection:
    case IntersectionSelection:
    {
        QPointF pf = invTransform(p);
        QPolygonF tmp = m_polygon;
        tmp.append(pf);
        if(m_tmpItem)
        {
            m_tmpItem->setPolygon(tmp);
        }
        break;
    }
    default:
        return false;
    }
    return false;//把移动的事件继续传递下去
}


bool SAPolygonRegionSelectEditor::keyPressEvent(const QKeyEvent *e)
{
    if(Qt::Key_Enter == e->key() || Qt::Key_Return == e->key())
    {
        return completeRegion();
    }
    else if(Qt::Key_Escape == e->key() || Qt::Key_Backspace == e->key() )
    {
        return backspaceRegion();
    }
    return false;
}

bool SAPolygonRegionSelectEditor::completeRegion()
{
    if(m_polygon.size()<=2)
    {
        m_polygon.clear();
        if(m_tmpItem)
        {
            m_tmpItem->detach();
            delete m_tmpItem;
            m_tmpItem = nullptr;
        }
        m_isStartDrawRegion = false;
        return false;//点数不足，完成失败
    }
    else
    {
        //点数足够，封闭多边形
        if(m_polygon.last() != m_polygon.first())
        {
            m_polygon.append(m_polygon.first());
        }
    }
    switch(getSelectionMode())
    {
    case SingleSelection:
    {
        if(m_shapeItem)
        {
            m_shapeItem->setPolygon(m_polygon);
        }
        else
        {
            //几乎无可能进入这里
            m_shapeItem = new SASelectRegionShapeItem("select region");
            m_shapeItem->attach(plot());
            m_shapeItem->setPolygon(m_polygon);
        }
        m_polygon.clear();
        break;
    }
    case AdditionalSelection:
    case SubtractionSelection:
    case IntersectionSelection:
    {
        if(m_shapeItem)
        {
            QPainterPath shape = m_shapeItem->shape();
            QPainterPath addtion;
            addtion.addPolygon(m_polygon);
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
        else
        {
            //几乎无可能进入这里
            m_shapeItem = new SASelectRegionShapeItem("select region");
            m_shapeItem->attach(plot());
            m_shapeItem->setPolygon(m_polygon);
        }
        m_polygon.clear();
        if(m_tmpItem)
        {
            m_tmpItem->detach();
            delete m_tmpItem;
            m_tmpItem = nullptr;
        }
        break;
    }
    }
    m_isStartDrawRegion = false;
    return true;
}

bool SAPolygonRegionSelectEditor::backspaceRegion()
{
    if(!m_isStartDrawRegion)
    {
        return false;
    }
    if(m_polygon.size()<=1)
    {
        return false;
    }
    m_polygon.pop_back();
    switch(getSelectionMode())
    {
    case SingleSelection:
    {
        if(m_shapeItem)
        {
            m_shapeItem->setPolygon(m_polygon);
        }
        break;
    }
    case AdditionalSelection:
    case SubtractionSelection:
    case IntersectionSelection:
    {
        if(m_tmpItem)
        {
            m_tmpItem->setPolygon(m_polygon);
        }
        break;
    }
    default:
        return false;
    }
    return true;
}
