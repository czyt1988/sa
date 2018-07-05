#include "ui_opt.h"
#include "SAChart2D.h"
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include "QApplication"

#define TR(str)\
    QApplication::translate("ui_opt", str, 0)

SAChart2D* filter_xy_series(SAUIInterface *ui, QList<QwtPlotItem*>& res)
{
    SAChart2D* chart = ui->getCurSubWindowChart();
    if(!chart)
    {
        ui->showWarningMessageInfo(TR("you should select a chart at first"));
        ui->raiseMessageInfoDock();
        return chart;
    }
    QList<QwtPlotItem*> curs = chart->getCurrentSelectItems();
    if(0 == curs.size())
    {
        curs = ui->selectPlotItems(chart,SAChart2D::getXYSeriesItemsRTTI().toSet());
    }
    if(curs.size() <= 0)
    {
        return chart;
    }
    res = curs;
    return chart;
}
