#include "SALineGradientColorList.h"
#include "SAColorAlgorithm.h"
SALineGradientColorList::SALineGradientColorList()
{
}


SALineGradientColorList::SALineGradientColorList(const QColor& start, const QColor& end, unsigned int step)
{
    setColorRange(start, end, step);
}


void SALineGradientColorList::setColorRange(const QColor& start, const QColor& end, unsigned int step)
{
    m_colorlist = SA::generate_line_gradientcolor(start, end, step);
}


QColor SALineGradientColorList::getColor(double percent)
{
    int index = m_colorlist.size() * percent;

    if ((index >= m_colorlist.size()) || (index < 0)) {
        return (QColor());
    }
    return (m_colorlist.value(index));
}


QColor SALineGradientColorList::getColor(const QColor& start, const QColor& end, unsigned int step, double percent)
{
    return (SA::generate_line_gradientcolor(start, end, step, percent));
}
