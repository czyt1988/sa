#ifndef SAFIGURECHARTRUBBERBANDEDITOVERLAY_H
#define SAFIGURECHARTRUBBERBANDEDITOVERLAY_H
#include "SAFigureWindowOverlay.h"
class SAFigureChartRubberbandEditOverlayPrivate;
///
/// \brief The SAFigureChartRubberbandEditOverlay class
///
class SAFigureChartRubberbandEditOverlay : public SAFigureWindowOverlay
{
    Q_OBJECT
    SA_IMPL(SAFigureChartRubberbandEditOverlay)
public:
    SAFigureChartRubberbandEditOverlay(SAFigureWindow* fig);
    ~SAFigureChartRubberbandEditOverlay();
    enum RectRange
    {
        Top
        ,Bottom
        ,Left
        ,Right
        ,TopLeft
        ,TopRight
        ,BottomLeft
        ,BottomRight
        ,Inner
        ,OutSide
    };

public:
    virtual void drawChartEditMode(QPainter* painter, const QRect &rect) const;
    //根据点和矩形的关系，返回图标的样式
    static Qt::CursorShape rectRangeToCursorShape(RectRange rr);
    static RectRange getPointInRectRange(const QPoint& pos,const QRect& region,int err = 1);
    static bool isPointInRectEdget(const QPoint& pos,const QRect& region,int err = 1);
protected:
    virtual void drawOverlay( QPainter * p) const;
    virtual QRegion maskHint() const;
    virtual bool eventFilter(QObject *obj, QEvent *event);
};

#endif // SAFIGURECHARTRUBBERBANDEDITOVERLAY_H
