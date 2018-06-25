#include "SAFigureWindowOverlay.h"

SAFigureWindowOverlay::SAFigureWindowOverlay(SAFigureWindow* fig)
    :QwtWidgetOverlay(fig)
{

}

SAFigureWindow *SAFigureWindowOverlay::figure() const
{
    return qobject_cast<SAFigureWindow *>(parent());
}
