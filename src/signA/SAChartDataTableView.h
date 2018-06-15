#ifndef SACHARTDATATABLEVIEW_H
#define SACHARTDATATABLEVIEW_H
#include "SATableView.h"
#include <functional>

///
/// \brief The SAChartDataTableView class
///
class SAChartDataTableView : public SATableView
{
    Q_OBJECT
public:
    SAChartDataTableView(QWidget* par = Q_NULLPTR);
    //设置单元格高度
    void setCellHeight(int height);
};

#endif // SACHARTDATATABLEVIEW_H
