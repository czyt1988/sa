#include "SARectRegionSelectEditor.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>

SARectRegionSelectEditor::SARectRegionSelectEditor(QwtPlot *parent)
    :SAAbstractRegionSelectEditor(parent)
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
    if(m_tmpItem)
    {
        m_tmpItem->detach();
        delete m_tmpItem;
        m_tmpItem = nullptr;
    }
}



bool SARectRegionSelectEditor::mousePressEvent(const QMouseEvent *e)
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

bool SARectRegionSelectEditor::mouseMovedEvent(const QMouseEvent *e)
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
    default:
        return false;
    }
    return true;
}

bool SARectRegionSelectEditor::mouseReleasedEvent(const QMouseEvent *e)
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
                m_shapeItem->setRect(m_selectedRect);
            }
            else
            {
                //几乎无可能进入这里
                m_shapeItem = new SASelectRegionShapeItem("select region");
                m_shapeItem->attach(plot());
                m_shapeItem->setRect(m_selectedRect);
            }
            emit finishSelection(m_shapeItem->shape());
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
            QPainterPath shape = m_shapeItem->shape();
            if(m_shapeItem)
            {
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
            emit finishSelection(shape);
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
///
/// \brief 处理按钮事件
/// \param e
///
bool SARectRegionSelectEditor::keyPressEvent(const QKeyEvent *e)
{
    return SAAbstractRegionSelectEditor::keyPressEvent(e);
}
bool SARectRegionSelectEditor::keyReleaseEvent(const QKeyEvent *e)
{
    return SAAbstractRegionSelectEditor::keyReleaseEvent(e);
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
///
/// \brief SARectRegionSelectEditor::setSelectRegion
/// \param shape
///
void SARectRegionSelectEditor::setSelectRegion(const QPainterPath &shape)
{
    if(nullptr == m_shapeItem)
    {
        m_shapeItem = new SASelectRegionShapeItem("select region");
        m_shapeItem->setShape(shape);
        m_shapeItem->attach(plot());
    }
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
/// \brief rtti
/// \return
///
int SARectRegionSelectEditor::rtti() const
{
    return RTTIRectRegionSelectEditor;
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
    if(m_tmpItem)
    {
        m_tmpItem->detach();
        delete m_tmpItem;
        m_tmpItem = nullptr;
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


