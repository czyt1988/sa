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
public:

    virtual void drawChartEditMode(QPainter* painter, const QRect &rect) const;
protected:
    virtual void drawOverlay( QPainter * p) const;
    virtual QRegion maskHint() const;
    virtual bool eventFilter(QObject *obj, QEvent *event);
};

#endif // SAFIGURECHARTRUBBERBANDEDITOVERLAY_H
