#ifndef SAFIGUREWINDOWOVERLAY_H
#define SAFIGUREWINDOWOVERLAY_H
#include "qwt_widget_overlay.h"
#include "SAFigureWindow.h"
#include "SACommonUIGlobal.h"

///
/// \brief The SAFigureWindowOverlay class
///
class SA_COMMON_UI_EXPORT SAFigureWindowOverlay : public QwtWidgetOverlay
{
    Q_OBJECT
public:
    SAFigureWindowOverlay(SAFigureWindow* fig);
    SAFigureWindow* figure() const;
};

#endif // SAFIGUREWINDOWOVERLAY_H
