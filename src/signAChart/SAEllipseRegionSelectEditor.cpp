#include "SAEllipseRegionSelectEditor.h"
#include <QMouseEvent>
#include <QKeyEvent>
class SAEllipseRegionSelectEditorPrivate
{
    SA_IMPL_PUBLIC(SAEllipseRegionSelectEditor)
public:
    bool m_isStartDrawRegion;
    SASelectRegionShapeItem* m_tmpItem;
    QPointF m_pressedPoint;
    QRectF m_selectedRect;
    QPainterPath m_lastPainterPath;
    SAEllipseRegionSelectEditorPrivate(SAEllipseRegionSelectEditor* p):q_ptr(p)
      ,m_isStartDrawRegion(false)
      ,m_tmpItem(nullptr)
      ,m_selectedRect(QRectF())
    {

    }
    ~SAEllipseRegionSelectEditorPrivate()
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

SAEllipseRegionSelectEditor::SAEllipseRegionSelectEditor(QwtPlot *parent)
    :SAAbstractRegionSelectEditor(parent)
    ,d_ptr(new SAEllipseRegionSelectEditorPrivate(this))
{
    setEnabled(true);
    connect(parent,&QwtPlot::itemAttached,this,&SAEllipseRegionSelectEditor::onItemAttached);
}

SAEllipseRegionSelectEditor::~SAEllipseRegionSelectEditor()
{

}



QPainterPath SAEllipseRegionSelectEditor::getSelectRegion() const
{
    SA_DC(SAEllipseRegionSelectEditor);
    return d->m_lastPainterPath;
}

void SAEllipseRegionSelectEditor::setSelectRegion(const QPainterPath &shape)
{
    SA_D(SAEllipseRegionSelectEditor);
    d->m_lastPainterPath = shape;
}


void SAEllipseRegionSelectEditor::setSelectionMode(const SAAbstractRegionSelectEditor::SelectionMode &selectionMode)
{
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

///
/// \brief 清理数据
///
void SAEllipseRegionSelectEditor::clear()
{
    SA_D(SAEllipseRegionSelectEditor);
    d->releaseTmpItem();
    d->m_selectedRect = QRectF();
    d->m_lastPainterPath = QPainterPath();
}

void SAEllipseRegionSelectEditor::onItemAttached(QwtPlotItem *item, bool on)
{
    SA_D(SAEllipseRegionSelectEditor);
    if(!on)
    {
        if(item == d->m_tmpItem)
        {
            d->m_tmpItem = nullptr;
        }
    }
}

bool SAEllipseRegionSelectEditor::mousePressEvent(const QMouseEvent *e)
{
    SA_D(SAEllipseRegionSelectEditor);
    if(Qt::MidButton == e->button() || Qt::RightButton == e->button())
    {
        return false;
    }
    QPoint p = e->pos();
    d->m_isStartDrawRegion = true;
    d->m_pressedPoint = invTransform(p);
    d->createTmpItem();
    switch(getSelectionMode())
    {
    case SingleSelection://单一选择
    {
        d->m_lastPainterPath = QPainterPath();
        break;
    }
    default:
        return false;
    }
    return true;
}

bool SAEllipseRegionSelectEditor::mouseMovedEvent(const QMouseEvent *e)
{
    SA_D(SAEllipseRegionSelectEditor);
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
    d->m_selectedRect.setX(d->m_pressedPoint.x());
    d->m_selectedRect.setY(d->m_pressedPoint.y());
    d->m_selectedRect.setWidth(pf.x() - d->m_pressedPoint.x());
    d->m_selectedRect.setHeight(pf.y() - d->m_pressedPoint.y());
    if(d->m_tmpItem)
    {
        d->m_tmpItem->setEllipse(d->m_selectedRect);
    }
    return true;
}

bool SAEllipseRegionSelectEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    SA_D(SAEllipseRegionSelectEditor);
    if(Qt::MidButton == e->button() || Qt::RightButton == e->button())
    {
        return false;
    }
    QPoint p = e->pos();
    QPointF pf = invTransform(p);
    if(pf == d->m_pressedPoint)
    {
        //如果点击和松开是一个点，就取消当前的选区
        d->releaseTmpItem();
        d->m_isStartDrawRegion = false;
        return true;
    }
    d->m_selectedRect.setX(d->m_pressedPoint.x());
    d->m_selectedRect.setY(d->m_pressedPoint.y());
    d->m_selectedRect.setWidth(pf.x() - d->m_pressedPoint.x());
    d->m_selectedRect.setHeight(pf.y() - d->m_pressedPoint.y());
    QPainterPath painterPath;
    painterPath.addEllipse(d->m_selectedRect);
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
    d->m_isStartDrawRegion = false;
    emit finishSelection(d->m_lastPainterPath);
    return true;
}


