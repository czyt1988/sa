#include "SAPolygonRegionSelectEditor.h"
#include <QMouseEvent>
#include <QKeyEvent>
class SAPolygonRegionSelectEditorPrivate
{
    SA_IMPL_PUBLIC(SAPolygonRegionSelectEditor)
public:
    bool m_isStartDrawRegion;///< 是否生效
    SASelectRegionShapeItem* m_tmpItem;
    QPolygonF m_polygon;///< 多边形
    bool m_isFinishOneRegion;///< 标定是否已经完成了一次区域，m_tmpItem还是m_shapeItem显示
    QPainterPath m_lastPainterPath;

    SAPolygonRegionSelectEditorPrivate(SAPolygonRegionSelectEditor* p):q_ptr(p)
      ,m_isStartDrawRegion(false)
      ,m_tmpItem(nullptr)
      ,m_isFinishOneRegion(false)
    {

    }
    ~SAPolygonRegionSelectEditorPrivate()
    {
        releaseTmpItem();
    }

    void releaseTmpItem()
    {
        if(m_tmpItem)
        {
            m_tmpItem->detach();
            delete m_tmpItem;
            m_tmpItem = nullptr;
        }
    }
    void createTmpItem()
    {
        if(nullptr == m_tmpItem)
        {
            m_tmpItem = new SASelectRegionShapeItem("temp region");
            m_tmpItem->attach(q_ptr->plot());
        }
    }
};

SAPolygonRegionSelectEditor::SAPolygonRegionSelectEditor(QwtPlot* parent)
    :SAAbstractRegionSelectEditor(parent)
    ,d_ptr(new SAPolygonRegionSelectEditorPrivate(this))
{
    setEnabled(true);
    connect(parent,&QwtPlot::itemAttached,this,&SAPolygonRegionSelectEditor::onItemAttached);
}

SAPolygonRegionSelectEditor::~SAPolygonRegionSelectEditor()
{

}

QPainterPath SAPolygonRegionSelectEditor::getSelectRegion() const
{
    SA_DC(SAPolygonRegionSelectEditor);
    return d->m_lastPainterPath;
}

void SAPolygonRegionSelectEditor::setSelectRegion(const QPainterPath &shape)
{
    SA_D(SAPolygonRegionSelectEditor);
    d->m_lastPainterPath = shape;
}

void SAPolygonRegionSelectEditor::setSelectionMode(const SAAbstractRegionSelectEditor::SelectionMode &selectionMode)
{
    SAAbstractRegionSelectEditor::setSelectionMode(selectionMode);
}

int SAPolygonRegionSelectEditor::rtti() const
{
    return RTTIPolygonRegionSelectEditor;
}

void SAPolygonRegionSelectEditor::onItemAttached(QwtPlotItem *item, bool on)
{
    SA_D(SAPolygonRegionSelectEditor);
    if(!on)
    {
        if(item == d->m_tmpItem)
        {
            d->m_tmpItem = nullptr;
        }
    }
}

bool SAPolygonRegionSelectEditor::mousePressEvent(const QMouseEvent *e)
{
    SA_D(SAPolygonRegionSelectEditor);
    if(Qt::MidButton == e->button() || Qt::RightButton == e->button())
    {
        return false;
    }
    QPoint p = e->pos();
    d->m_isStartDrawRegion = true;
    d->createTmpItem();
    d->m_polygon.append(invTransform(p));
    if(d->m_tmpItem)
    {
        d->m_tmpItem->setPolygon(d->m_polygon);
    }
    return true;
}

bool SAPolygonRegionSelectEditor::mouseMovedEvent(const QMouseEvent *e)
{
    SA_D(SAPolygonRegionSelectEditor);
    if(!d->m_isStartDrawRegion)
    {
        return false;
    }
    if(Qt::MidButton == e->button() || Qt::RightButton == e->button())
    {
        return false;
    }
    QPoint p = e->pos();
    QPointF pf = invTransform(p);
    QPolygonF tmp = d->m_polygon;
    tmp.append(pf);
    if(d->m_tmpItem)
    {
        d->m_tmpItem->setPolygon(tmp);
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
    SA_D(SAPolygonRegionSelectEditor);
    if(d->m_polygon.size()<=2)
    {
        d->m_polygon.clear();
        d->releaseTmpItem();
        d->m_isStartDrawRegion = false;
        return false;//点数不足，完成失败
    }
    else
    {
        //点数足够，封闭多边形
        if(d->m_polygon.last() != d->m_polygon.first())
        {
            d->m_polygon.append(d->m_polygon.first());
        }
    }
    QPainterPath painterPath;
    painterPath.addPolygon(d->m_polygon);
    switch(getSelectionMode())
    {
    case SingleSelection:
    {
        d->m_lastPainterPath = painterPath;
        break;
    }
    case AdditionalSelection:
    {
        d->m_lastPainterPath = d->m_lastPainterPath.united(painterPath);
        break;
    }
    case SubtractionSelection:
    {
        d->m_lastPainterPath = d->m_lastPainterPath.subtracted(painterPath);
        break;
    }
    case IntersectionSelection:
    {
        d->m_lastPainterPath = d->m_lastPainterPath.intersected(painterPath);
        break;
    }
    default:
        break;
    }
    d->releaseTmpItem();
    d->m_polygon.clear();
    d->m_isStartDrawRegion = false;
    emit finishSelection(d->m_lastPainterPath);
    return true;
}
///
/// \brief 回退
/// \return
///
bool SAPolygonRegionSelectEditor::backspaceRegion()
{
    SA_D(SAPolygonRegionSelectEditor);
    if(!d->m_isStartDrawRegion)
    {
        return false;
    }
    if(d->m_polygon.size()<=1)
    {
        return false;
    }
    d->m_polygon.pop_back();
    if(d->m_tmpItem)
    {
        d->m_tmpItem->setPolygon(d->m_polygon);
    }
    return true;
}
