#include "SAFigureWindowOverlay.h"
#include <QDebug>
SAFigureWindowOverlay::SAFigureWindowOverlay(SAFigureWindow* fig)
    :QwtWidgetOverlay(fig)
    ,m_fig(nullptr)
{
    m_fig = fig;
}

SAFigureWindow *SAFigureWindowOverlay::figure() const
{
    return m_fig;
}
