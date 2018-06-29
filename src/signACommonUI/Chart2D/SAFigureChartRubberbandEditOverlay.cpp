#include "SAFigureChartRubberbandEditOverlay.h"
#include "SAChart2D.h"
#include <QMouseEvent>
#include <QHash>

struct private_widget_data
{
    SAFigureChartRubberbandEditOverlay::RectRange rectRange;

};


class SAFigureChartRubberbandEditOverlayPrivate
{
    SA_IMPL_PUBLIC(SAFigureChartRubberbandEditOverlay)
public:
    QBrush chart2dEditModeContorlPointBrush;///< 绘制chart2d在编辑模式下控制点的画刷
    QPen chart2dEditModeEdgetPen;///< 绘制chart2d在编辑模式下的画笔
    bool isStartResize;///< 标定开始进行缩放
    QWidget* activeWidget;///标定当前激活的窗口，如果没有就为nullptr
    SAFigureChartRubberbandEditOverlay::RectRange activeWidgetRectRange;///< 记录当前缩放窗口的位置情况
    SAFigureChartRubberbandEditOverlayPrivate(SAFigureChartRubberbandEditOverlay* p):q_ptr(p)
      ,chart2dEditModeContorlPointBrush(Qt::blue)
      ,chart2dEditModeEdgetPen(Qt::blue)
    {

    }
};


SAFigureChartRubberbandEditOverlay::SAFigureChartRubberbandEditOverlay(SAFigureWindow *fig)
    :SAFigureWindowOverlay(fig)
    ,d_ptr(new SAFigureChartRubberbandEditOverlayPrivate(this))
{
   fig->installEventFilter(this);
}

SAFigureChartRubberbandEditOverlay::~SAFigureChartRubberbandEditOverlay()
{

}


void SAFigureChartRubberbandEditOverlay::drawChartEditMode(QPainter *painter,const QRect& rect) const
{
    painter->setBrush(Qt::NoBrush);
    QRect edgetRect = rect.adjusted(-1,-1,1,1);
    painter->drawRect(edgetRect);
    //绘制四个角落
    painter->setBrush(d_ptr->chart2dEditModeContorlPointBrush);
    QRect connerRect(0,0,5,5);
    connerRect.moveTo(edgetRect.topLeft()-QPoint(2,2));
    painter->drawRect(connerRect);
    connerRect.moveTo(edgetRect.topRight()-QPoint(2,2));
    painter->drawRect(connerRect);
    connerRect.moveTo(edgetRect.bottomLeft()-QPoint(2,2));
    painter->drawRect(connerRect);
    connerRect.moveTo(edgetRect.bottomRight()-QPoint(2,2));
    painter->drawRect(connerRect);


}
///
/// \brief 根据范围获取图标
/// \param rr 范围
/// \return 图标
///
Qt::CursorShape SAFigureChartRubberbandEditOverlay::rectRangeToCursorShape(SAFigureChartRubberbandEditOverlay::RectRange rr)
{
    switch(rr)
    {
    case Top:
    case Bottom:
        return Qt::SizeVerCursor;
    case Left:
    case Right:
        return Qt::SizeHorCursor;
    case TopLeft:
    case BottomRight:
        return Qt::SizeFDiagCursor;
    case TopRight:
    case BottomLeft:
        return Qt::SizeBDiagCursor;
    default:
        break;
    }
    return Qt::ArrowCursor;
}
///
/// \brief 根据点和矩形的关系，返回图标的样式
/// \param pos 点
/// \param region 矩形区域
/// \param err 允许误差
/// \return
///
SAFigureChartRubberbandEditOverlay::RectRange SAFigureChartRubberbandEditOverlay::getPointInRectRange(const QPoint &pos, const QRect &region, int err)
{
    if(!region.adjusted(-err,-err,err,err).contains(pos,true))
    {
        return OutSide;
    }
    if(pos.x() < (region.left()+err))
    {
        if(pos.y() < region.top()+err)
        {
            return TopLeft;
        }
        else if(pos.y() > region.bottom()-err)
        {
            return BottomLeft;
        }
        return Left;
    }
    else if(pos.x() > (region.right()-err))
    {
        if(pos.y() < region.top()+err)
        {
            return TopRight;
        }
        else if(pos.y() > region.bottom()-err)
        {
            return BottomRight;
        }
        return Right;
    }
    else if(pos.y() < (region.top()+err))
    {
        if(pos.x() < region.left()+err)
        {
            return TopLeft;
        }
        else if(pos.x() > region.right()-err)
        {
            return TopRight;
        }
        return Top;
    }
    else if(pos.y() > region.bottom()-err)
    {
        if(pos.x() < region.left()+err)
        {
            return BottomLeft;
        }
        else if(pos.x() > region.right()-err)
        {
            return BottomRight;
        }
        return Bottom;
    }
    return Inner;
}
///
/// \brief 判断点是否在矩形区域的边缘
/// \param pos 点
/// \param region 矩形区域
/// \param err 允许误差
/// \return 如果符合边缘条件，返回true
///
bool SAFigureChartRubberbandEditOverlay::isPointInRectEdget(const QPoint &pos, const QRect &region, int err)
{
    if(!region.adjusted(-err,-err,err,err).contains(pos))
    {
        return false;
    }
    if((pos.x() < (region.left()-err)) && (pos.x() < (region.left()+err)))
    {
        return true;
    }
    else if((pos.x() > (region.right()-err)) && (pos.x() < (region.right()+err)))
    {
        return true;
    }
    else if((pos.y() > (region.top()-err)) && (pos.y() < (region.top()+err)))
    {
        return true;
    }
    else if((pos.y() > region.bottom()-err) && (pos.y() < region.bottom()+err))
    {
        return true;
    }
    return false;
}

void SAFigureChartRubberbandEditOverlay::drawOverlay(QPainter *p) const
{
    p->save();
    p->setPen(d_ptr->chart2dEditModeEdgetPen);
    QList<SAChart2D*> plots2D = figure()->get2DPlots();
    for(int i=0;i<plots2D.size();++i)
    {
        drawChartEditMode(p,plots2D[i]->geometry());
    }
    p->restore();
}

QRegion SAFigureChartRubberbandEditOverlay::maskHint() const
{
    return figure()->rect();
}

bool SAFigureChartRubberbandEditOverlay::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonPress:
    {
        QMouseEvent* me = static_cast<QMouseEvent*>(event);
        if(Qt::LeftButton == me->button())
        {
            d_ptr->isStartResize = true;
        }
        return true;
    }
    case QEvent::MouseButtonRelease:
    {
        QMouseEvent* me = static_cast<QMouseEvent*>(event);
        if(Qt::LeftButton == me->button())
        {
            d_ptr->isStartResize = false;
        }
        return true;
    }
    case QEvent::MouseMove:
    {
        if(d_ptr->isStartResize)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            if(d_ptr->activeWidget)
            {
                switch (d_ptr->activeWidgetRectRange) {
                case Top:

                    break;
                default:
                    break;
                }
            }
        }
        return true;
    }
    case QEvent::Leave:
    {
        break;
    }
    case QEvent::HoverMove:
    {
        QHoverEvent* me = static_cast<QHoverEvent*>(event);
        qDebug() << "QEvent::HoverMove:" << me->pos();
        QList<SAChart2D*> plots2D = figure()->get2DPlots();
        for(int i=0;i<plots2D.size();++i)
        {
            RectRange rectRange = getPointInRectRange(me->pos(),plots2D[i]->frameGeometry(),4);
            Qt::CursorShape shape = rectRangeToCursorShape(rectRange);
            if(Qt::ArrowCursor == shape)
            {
                figure()->unsetCursor();
                d_ptr->activeWidget = nullptr;
                continue;
            }

            figure()->setCursor(shape);
            d_ptr->activeWidget = plots2D[i];
            d_ptr->activeWidgetRectRange = rectRange;
            //找到第一个窗口就跳出
            return true;

        }
        return true;
    }
    case QEvent::MouseButtonDblClick:
    {
        break;
    }
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}


