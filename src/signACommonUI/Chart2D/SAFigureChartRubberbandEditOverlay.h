#ifndef SAFIGURECHARTRUBBERBANDEDITOVERLAY_H
#define SAFIGURECHARTRUBBERBANDEDITOVERLAY_H
#include "SAFigureWindowOverlay.h"
class SAFigureChartRubberbandEditOverlayPrivate;
///
/// \brief 用于辅助显示figure的子chart位置编辑的覆盖辅助窗体
///
class SAFigureChartRubberbandEditOverlay : public SAFigureWindowOverlay
{
    Q_OBJECT
    SA_IMPL(SAFigureChartRubberbandEditOverlay)
public:
    SAFigureChartRubberbandEditOverlay(SAFigureWindow *fig);
    ~SAFigureChartRubberbandEditOverlay();
    ///
    /// \brief 用于标记矩形的区域
    ///
    enum RectRange {
        Top
        , Bottom
        , Left
        , Right
        , TopLeft
        , TopRight
        , BottomLeft
        , BottomRight
        , Inner
        , OutSide
    };

public:
    virtual void drawChartEditMode(QPainter *painter, const QRect& rect) const;

    //根据点和矩形的关系，返回图标的样式
    static Qt::CursorShape rectRangeToCursorShape(RectRange rr);
    static RectRange getPointInRectRange(const QPoint& pos, const QRect& region, int err = 1);
    static bool isPointInRectEdget(const QPoint& pos, const QRect& region, int err = 1);
    void selectNextWidget(bool forward = true);

protected:
    virtual void drawOverlay(QPainter *p) const;
    virtual QRegion maskHint() const;
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    bool onMouseMoveEvent(QMouseEvent *me);
    bool onMouseReleaseEvent(QMouseEvent *me);
    bool onMousePressedEvent(QMouseEvent *me);
    bool onMouseHoverMoveEvent(QHoverEvent *me);
    bool onKeyPressedEvent(QKeyEvent *ke);
};

#endif // SAFIGURECHARTRUBBERBANDEDITOVERLAY_H
