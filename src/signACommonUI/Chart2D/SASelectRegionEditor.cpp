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
    bool m_isStartMouseAction;///< 是否鼠标激活
    bool m_isStartKeyAction;///< 是否按键激活
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
///
/// \brief 选区进行移动
/// \param offset
///
void SASelectRegionEditor::offsetRegion(const QPointF &offset)
{
    QPainterPath p = chart2D()->getSelectionRange();
    p.translate(offset);
    chart2D()->setSelectionRange(p);
}

bool SASelectRegionEditor::completeEdit(const QPoint &screenPoint)
{
    if(!d_ptr->m_isStart)
    {
        plot()->unsetCursor();
        return false;
    }
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        plot()->unsetCursor();
        return false;
    }
    plot()->unsetCursor();
    d_ptr->m_isStart = false;
    d_ptr->m_isStartKeyAction = false;
    if(d_ptr->m_firstPressedScreenPoint == screenPoint)
    {
        d_ptr->m_tmpPoint = screenPoint;
        return true;
    }
    bool isAutoReplot = chart2D()->autoReplot();
    chart2D()->setAutoReplot(false);
    //选区进行移动
    QPointF currentPoint = editor->invTransform(screenPoint);
    QPointF originPoint = editor->invTransform(d_ptr->m_firstPressedScreenPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);
    QPainterPath p = d_ptr->m_selectRegionOrigin.translated(offset);
    SAFigureOptCommand* cmd = new SAFigureChartSelectionRegionAddCommand(chart2D()
                                                ,d_ptr->m_selectRegionOrigin
                                                ,p
                                                ,tr("move region")
                                                );
    //
    chart2D()->appendCommand(cmd);
    chart2D()->setAutoReplot(isAutoReplot);
    d_ptr->m_firstPressedScreenPoint = screenPoint;
    d_ptr->m_tmpPoint = screenPoint;
    return true;
}

void SASelectRegionEditor::startEdit(const QPoint &screenPoint)
{
    plot()->setCursor(QCursor(Qt::SizeAllCursor));
    d_ptr->m_isStart = true;
    d_ptr->m_firstPressedScreenPoint = screenPoint;
    d_ptr->m_tmpPoint = screenPoint;
    d_ptr->m_selectRegionOrigin = chart2D()->getSelectionRange();
}

void SASelectRegionEditor::moveEdit(const QPoint &toScreenPoint)
{
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return;
    }
    bool isAutoReplot = chart2D()->autoReplot();
    chart2D()->setAutoReplot(false);
    QPointF currentPoint = editor->invTransform(toScreenPoint);
    QPointF originPoint = editor->invTransform(d_ptr->m_tmpPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);
    d_ptr->m_tmpPoint = toScreenPoint;
    //选区进行移动
    offsetRegion(offset);
    chart2D()->setAutoReplot(isAutoReplot);
}

void SASelectRegionEditor::completeKeyActionEdit()
{
    if(d_ptr->m_isStartKeyAction)
    {
        completeEdit(d_ptr->m_tmpPoint);
    }
}








bool SASelectRegionEditor::mousePressEvent(const QMouseEvent *e)
{
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    completeKeyActionEdit();
    d_ptr->m_isStartMouseAction = true;
    startEdit(e->pos());
    return true;
}

bool SASelectRegionEditor::mouseMovedEvent(const QMouseEvent *e)
{
    if(!d_ptr->m_isStart || d_ptr->m_isStartKeyAction)
    {
        return false;
    }
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    moveEdit(e->pos());
    plot()->replot();
    return true;
}

bool SASelectRegionEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    d_ptr->m_isStartMouseAction = false;
    return completeEdit(e->pos());
}

bool SASelectRegionEditor::keyPressEvent(const QKeyEvent *e)
{
    //响应方向键和回车键
    if(d_ptr->m_isStartMouseAction)
    {
        return false;
    }
    if(Qt::ControlModifier == e->modifiers())
    {
        //Ctrl 按下，进行精细移动
        switch(e->key())
        {
        case Qt::Key_Up:
        case Qt::Key_Right:
        case Qt::Key_Left:
        case Qt::Key_Down:
        {
            if(!d_ptr->m_isStartKeyAction)
            {
                //说明是第一次进行按键操作
                QPoint virtualScreenPoint = chart2D()->rect().center();
                startEdit(virtualScreenPoint);
                d_ptr->m_isStartKeyAction = true;
            }
            break;
        }
        case Qt::Key_Enter://按下回车键确认键盘的移动
            completeKeyActionEdit();
            return true;
        default:
            return false;
        }

        switch(e->key())
        {
        case Qt::Key_Up:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() += 1;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Right:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.rx() += 1;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Left:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.rx() -= 1;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Down:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() -= 1;
            moveEdit(toPoint);
            break;
        }
        default:
            return false;
        }
        plot()->replot();
        return true;
    }
    else
    {
        //Ctrl没按下，进行粗移动
        switch(e->key())
        {
        case Qt::Key_Up:
        case Qt::Key_Right:
        case Qt::Key_Left:
        case Qt::Key_Down:
        {
            if(!d_ptr->m_isStartKeyAction)
            {
                //说明是第一次进行按键操作
                QPoint virtualScreenPoint = chart2D()->rect().center();
                startEdit(virtualScreenPoint);
                d_ptr->m_isStartKeyAction = true;
            }
            break;
        }
        //case Qt::Key
        default:
            return false;
        }
        int dw = plot()->width()/20;
        if(dw<=1)
        {
            dw = 1;
        }
        int dh = plot()->height()/20;
        if(dh<=1)
        {
            dh = 1;
        }

        switch(e->key())
        {
        case Qt::Key_Up:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() -= dh;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Right:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.rx() += dw;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Left:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.rx() -= dw;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Down:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() += dh;
            moveEdit(toPoint);
            break;
        }
        default:
            return false;
        }
        plot()->replot();
        return true;
    }
    return false;
}

bool SASelectRegionEditor::keyReleaseEvent(const QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}
