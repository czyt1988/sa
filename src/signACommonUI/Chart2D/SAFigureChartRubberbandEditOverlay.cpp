#include "SAFigureChartRubberbandEditOverlay.h"
#include "SAChart2D.h"
class SAFigureChartRubberbandEditOverlayPrivate
{
    SA_IMPL_PUBLIC(SAFigureChartRubberbandEditOverlay)
public:
    QBrush chart2dEditModeContorlPointBrush;///< 绘制chart2d在编辑模式下控制点的画刷
    QPen chart2dEditModeEdgetPen;///< 绘制chart2d在编辑模式下的画笔

    SAFigureChartRubberbandEditOverlayPrivate(SAFigureChartRubberbandEditOverlay* p):q_ptr(p)
      ,chart2dEditModeContorlPointBrush(Qt::blue)
      ,chart2dEditModeEdgetPen(Qt::blue)
    {

    }
};


SAFigureChartRubberbandEditOverlay::SAFigureChartRubberbandEditOverlay(SAFigureWindow *fig)
    :SAFigureWindowOverlay(fig)
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

    }
    case QEvent::MouseButtonRelease:
    {

    }
    case QEvent::MouseMove:
    {

    }
    case QEvent::Leave:
    {

    }
    case QEvent::HoverMove:
    {

    }
    case QEvent::MouseButtonDblClick:
    {

    }
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}


