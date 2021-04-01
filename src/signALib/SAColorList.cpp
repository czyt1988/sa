#include "SAColorList.h"
#include "SAColorAlgorithm.h"


SAColorList::SAColorList():QVector<QColor>()
{

}

SAColorList::SAColorList(int s):QVector<QColor>(s)
{

}

SAColorList::~SAColorList()
{

}


SAColorList::SAColorList(const QColor& startclr, const QColor& endclr, unsigned int step)
{
    setColorRange(startclr, endclr, step);
}


void SAColorList::setColorRange(const QColor& startclr, const QColor& endclr, unsigned int step)
{
    resize(step);
    SA::q_generate_line_gradientcolor(startclr, endclr, begin(),end());
}

/**
 * @brief SAColorList::getColor
 * @param percent 必须是0~1之间的数
 * @return
 */
QColor SAColorList::getColor(double percent) const
{
    return (value(size() * percent));
}


QColor SAColorList::getColor(const QColor& startclr, const QColor& endclr, unsigned int step, double percent)
{
    return (SA::q_generate_line_gradientcolor(startclr, endclr, step, percent));
}
