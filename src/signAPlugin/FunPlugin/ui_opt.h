#ifndef UI_OPT_H
#define UI_OPT_H
#include <QList>
class QwtPlotItem;
class SAChart2D;
class SAUIInterface;

SAChart2D* filter_xy_series(SAUIInterface* ui,QList<QwtPlotItem*>& res);



#endif // UI_OPT_H
