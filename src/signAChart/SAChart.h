#ifndef SACHART_H
#define SACHART_H
#include "SAChartGlobals.h"
#include "SASeries.h"
#include <QColor>
class QwtPlotItem;
///
/// \brief 这是一个辅助类，用于绘图的辅助
///
class SA_CHART_EXPORT SAChart
{
public:
    enum Value{
        Nan = -1
    };
    //获取item的颜色,无法获取单一颜色就返回QColor()
    static QColor getItemColor(QwtPlotItem* item);
    //获取item的数据个数，-1为nan
    static int getItemDataSize(QwtPlotItem* item);
};

#endif // SACHART_H
