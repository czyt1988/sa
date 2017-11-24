#include "SASelectRegionEditor.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAAbstractRegionSelectEditor.h"
#include "czyQtPointAlgorithm.h"
#include <QHash>
#include "SALog.h"
#include "SAFigureOptCommands.h"


class SASelectRegionEditorPrivate
{
    SA_IMPL_PUBLIC(SASelectRegionEditor)
public:
    bool m_isStart;//是否开始
    QPainterPath m_selectRegionOrigin;//保存选择的原始区域
    QPoint m_firstPressedScreenPoint;//第一次按下的点
    QPoint m_tmpPoint;//记录临时点
    SASelectRegionEditorPrivate(SASelectRegionEditor* p)
        :q_ptr(p)
        ,m_isStart(false)
    {

    }
};


SASelectRegionEditor::SASelectRegionEditor(SAChart2D *parent)
    :SAAbstractPlotEditor(parent)
    ,d_ptr(new SASelectRegionEditorPrivate(this))
{
    setEnabled(true);
}

SASelectRegionEditor::~SASelectRegionEditor()
{

}



int SASelectRegionEditor::rtti() const
{
    return RTTISelectRegionEditor;
}

const SAChart2D *SASelectRegionEditor::chart2D() const
{
    return qobject_cast<const SAChart2D*>(plot());
}

SAChart2D *SASelectRegionEditor::chart2D()
{
    return qobject_cast<SAChart2D*>(plot());
}








bool SASelectRegionEditor::mousePressEvent(const QMouseEvent *e)
{
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    d_ptr->m_isStart = true;
    d_ptr->m_firstPressedScreenPoint = e->pos();
    d_ptr->m_tmpPoint = e->pos();
    d_ptr->m_selectRegionOrigin = chart2D()->getSelectionRange();
    return true;
}

bool SASelectRegionEditor::mouseMovedEvent(const QMouseEvent *e)
{
    if(!d_ptr->m_isStart)
    {
        return false;
    }

    QPoint screenPoint = e->pos();
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    chart2D()->setAutoReplot(false);
    QPointF currentPoint = editor->invTransform(screenPoint);
    QPointF originPoint = editor->invTransform(d_ptr->m_tmpPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);
    d_ptr->m_tmpPoint = screenPoint;
    //选区进行移动
    QPainterPath p = chart2D()->getSelectionRange();
    p.translate(offset);
    chart2D()->setSelectionRange(p);
    plot()->replot();
    return true;
}

bool SASelectRegionEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    if(!d_ptr->m_isStart)
    {
        return false;
    }
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    chart2D()->setAutoReplot(false);
    QPointF currentPoint = editor->invTransform(e->pos());
    QPointF originPoint = editor->invTransform(d_ptr->m_firstPressedScreenPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);
    QPainterPath p = d_ptr->m_selectRegionOrigin.translated(offset);
    chart2D()->appendCommand(new SAFigureChartSelectionRegionAddCommand(chart2D(),d_ptr->m_selectRegionOrigin,p,tr("move region")));
    chart2D()->setAutoReplot(true);
    plot()->replot();
    d_ptr->m_isStart = false;
    return true;
}

bool SASelectRegionEditor::keyPressEvent(const QKeyEvent *e)
{
    return false;
}

bool SASelectRegionEditor::keyReleaseEvent(const QKeyEvent *e)
{
    return false;
}
