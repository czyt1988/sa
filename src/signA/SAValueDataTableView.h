#ifndef SAVALUEDATATABLEVIEW_H
#define SAVALUEDATATABLEVIEW_H
#include "SATableView.h"
#include <functional>
class QKeyEvent;
class SAValueDataTableViewPrivate;
///
/// \brief 用于数据显示的table view
///
class SAValueDataTableView : public SATableView
{
    Q_OBJECT
public:
    SAValueDataTableView(QWidget* par = Q_NULLPTR);
    //设置单元格高度
    void setCellHeight(int height);
};

#endif // SADATATABLEVIEW_H
