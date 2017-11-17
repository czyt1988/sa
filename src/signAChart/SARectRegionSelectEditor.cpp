#include "SARectRegionSelectEditor.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>
class SARectRegionSelectEditorPrivate
{
    SA_IMPL_PUBLIC(SARectRegionSelectEditor)
    bool m_isStartDrawRegion;
    SASelectRegionShapeItem* m_tmpItem;
    QPointF m_pressedPoint;
    QRectF m_selectedRect;
    QPainterPath m_lastPainterPath;
public:
    SARectRegionSelectEditorPrivate(SARectRegionSelectEditor* p):q_ptr(p)
      ,m_isStartDrawRegion(false)
      ,m_tmpItem(nullptr)
      ,m_selectedRect(QRectF())
    {

    }
    ~SARectRegionSelectEditorPrivate()
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

SARectRegionSelectEditor::SARectRegionSelectEditor(QwtPlot *parent)
    :SAAbstractRegionSelectEditor(parent)
    ,d_ptr(new SARectRegionSelectEditorPrivate(this))
{
    setEnabled(true);
    connect(parent,&QwtPlot::itemAttached,this,&SARectRegionSelectEditor::onItemAttached);
}

SARectRegionSelectEditor::~SARectRegionSelectEditor()
{

}



bool SARectRegionSelectEditor::mousePressEvent(const QMouseEvent *e)
{
    SA_D(SARectRegionSelectEditor);
    if(Qt::MidButton == e->button() || Qt::RightButton == e->button())
    {
        return false;
    }
    QPoint p = e->pos();
    d->createTmpItem();
    d->m_isStartDrawRegion = true;
    d->m_pressedPoint = invTransform(p);
    switch(getSelectionMode())
    {
    case SingleSelection:
    {
        d->m_lastPainterPath = QPainterPath();
    }
    default:
        break;
    }
    return true;
}

bool SARectRegionSelectEditor::mouseMovedEvent(const QMouseEvent *e)
{
    SA_D(SARectRegionSelectEditor);
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
        d->m_tmpItem->setRect(d->m_selectedRect);
    }
    return true;
}

bool SARectRegionSelectEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    SA_D(SARectRegionSelectEditor);
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
    painterPath.addRect(d->m_selectedRect);
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
/// \brief 获取选择的数据区域
/// \return
///
QPainterPath SARectRegionSelectEditor::getSelectRegion() const
{
    SA_DC(SARectRegionSelectEditor);
    return d->m_lastPainterPath;
}
///
/// \brief SARectRegionSelectEditor::setSelectRegion
/// \param shape
///
void SARectRegionSelectEditor::setSelectRegion(const QPainterPath &shape)
{
    SA_D(SARectRegionSelectEditor);
    d->m_lastPainterPath = shape;
}

///
/// \brief 设置选择模式
/// \param selectionMode
///
void SARectRegionSelectEditor::setSelectionMode(const SARectRegionSelectEditor::SelectionMode &selectionMode)
{
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
    SA_D(SARectRegionSelectEditor);
    d->releaseTmpItem();
    d->m_selectedRect = QRectF();
    d->m_lastPainterPath = QPainterPath();
}

void SARectRegionSelectEditor::onItemAttached(QwtPlotItem *item, bool on)
{
    SA_D(SARectRegionSelectEditor);
    if(!on)
    {
        if(item == d->m_tmpItem)
        {
            d->m_tmpItem = nullptr;
        }
    }
}


